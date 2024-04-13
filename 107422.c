size_t streamReplyWithRange(client *c, stream *s, streamID *start, streamID *end, size_t count, int rev, streamCG *group, streamConsumer *consumer, int flags, streamPropInfo *spi) {
    void *arraylen_ptr = NULL;
    size_t arraylen = 0;
    streamIterator si;
    int64_t numfields;
    streamID id;

    /* If a group was passed, we check if the request is about messages
     * never delivered so far (normally this happens when ">" ID is passed).
     *
     * If instead the client is asking for some history, we serve it
     * using a different function, so that we return entries *solely*
     * from its own PEL. This ensures each consumer will always and only
     * see the history of messages delivered to it and not yet confirmed
     * as delivered. */
    if (group && streamCompareID(start,&group->last_id) <= 0) {
        return streamReplyWithRangeFromConsumerPEL(c,s,start,end,count,
                                                   consumer);
    }

    if (!(flags & STREAM_RWR_RAWENTRIES))
        arraylen_ptr = addDeferredMultiBulkLength(c);
    streamIteratorStart(&si,s,start,end,rev);
    while(streamIteratorGetID(&si,&id,&numfields)) {
        /* Update the group last_id if needed. */
        if (group && streamCompareID(&id,&group->last_id) > 0)
            group->last_id = id;

        /* Emit a two elements array for each item. The first is
         * the ID, the second is an array of field-value pairs. */
        addReplyMultiBulkLen(c,2);
        addReplyStreamID(c,&id);
        addReplyMultiBulkLen(c,numfields*2);

        /* Emit the field-value pairs. */
        while(numfields--) {
            unsigned char *key, *value;
            int64_t key_len, value_len;
            streamIteratorGetField(&si,&key,&value,&key_len,&value_len);
            addReplyBulkCBuffer(c,key,key_len);
            addReplyBulkCBuffer(c,value,value_len);
        }

        /* If a group is passed, we need to create an entry in the
         * PEL (pending entries list) of this group *and* this consumer.
         *
         * Note that we cannot be sure about the fact the message is not
         * already owned by another consumer, because the admin is able
         * to change the consumer group last delivered ID using the
         * XGROUP SETID command. So if we find that there is already
         * a NACK for the entry, we need to associate it to the new
         * consumer. */
        if (group && !(flags & STREAM_RWR_NOACK)) {
            unsigned char buf[sizeof(streamID)];
            streamEncodeID(buf,&id);

            /* Try to add a new NACK. Most of the time this will work and
             * will not require extra lookups. We'll fix the problem later
             * if we find that there is already a entry for this ID. */
            streamNACK *nack = streamCreateNACK(consumer);
            int retval = 0;
            retval += raxTryInsert(group->pel,buf,sizeof(buf),nack,NULL);
            retval += raxTryInsert(consumer->pel,buf,sizeof(buf),nack,NULL);

            /* Now we can check if the entry was already busy, and
             * in that case reassign the entry to the new consumer. */
            if (retval == 0) {
                streamFreeNACK(nack);
                nack = raxFind(group->pel,buf,sizeof(buf));
                serverAssert(nack != raxNotFound);
                raxRemove(nack->consumer->pel,buf,sizeof(buf),NULL);
                /* Update the consumer and idle time. */
                nack->consumer = consumer;
                nack->delivery_time = mstime();
                nack->delivery_count++;
                /* Add the entry in the new consumer local PEL. */
                raxInsert(consumer->pel,buf,sizeof(buf),nack,NULL);
            } else if (retval == 1) {
                serverPanic("NACK half-created. Should not be possible.");
            }

            /* Propagate as XCLAIM. */
            if (spi) {
                robj *idarg = createObjectFromStreamID(&id);
                streamPropagateXCLAIM(c,spi->keyname,spi->groupname,idarg,nack);
                decrRefCount(idarg);
            }
        }

        arraylen++;
        if (count && count == arraylen) break;
    }
    streamIteratorStop(&si);
    if (arraylen_ptr) setDeferredMultiBulkLength(c,arraylen_ptr,arraylen);
    return arraylen;
}

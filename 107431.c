void xinfoCommand(client *c) {
    const char *help[] = {
"CONSUMERS <key> <groupname>  -- Show consumer groups of group <groupname>.",
"GROUPS <key>                 -- Show the stream consumer groups.",
"STREAM <key>                 -- Show information about the stream.",
"HELP                         -- Print this help.",
NULL
    };
    stream *s = NULL;
    char *opt;
    robj *key;

    /* HELP is special. Handle it ASAP. */
    if (!strcasecmp(c->argv[1]->ptr,"HELP")) {
        addReplyHelp(c, help);
        return;
    } else if (c->argc < 3) {
        addReplyError(c,"syntax error, try 'XINFO HELP'");
        return;
    }

    /* With the exception of HELP handled before any other sub commands, all
     * the ones are in the form of "<subcommand> <key>". */
    opt = c->argv[1]->ptr;
    key = c->argv[2];

    /* Lookup the key now, this is common for all the subcommands but HELP. */
    robj *o = lookupKeyWriteOrReply(c,key,shared.nokeyerr);
    if (o == NULL) return;
    s = o->ptr;

    /* Dispatch the different subcommands. */
    if (!strcasecmp(opt,"CONSUMERS") && c->argc == 4) {
        /* XINFO CONSUMERS <key> <group>. */
        streamCG *cg = streamLookupCG(s,c->argv[3]->ptr);
        if (cg == NULL) {
            addReplyErrorFormat(c, "-NOGROUP No such consumer group '%s' "
                                   "for key name '%s'",
                                   (char*)c->argv[3]->ptr, (char*)key->ptr);
            return;
        }

        addReplyMultiBulkLen(c,raxSize(cg->consumers));
        raxIterator ri;
        raxStart(&ri,cg->consumers);
        raxSeek(&ri,"^",NULL,0);
        mstime_t now = mstime();
        while(raxNext(&ri)) {
            streamConsumer *consumer = ri.data;
            mstime_t idle = now - consumer->seen_time;
            if (idle < 0) idle = 0;

            addReplyMultiBulkLen(c,6);
            addReplyStatus(c,"name");
            addReplyBulkCBuffer(c,consumer->name,sdslen(consumer->name));
            addReplyStatus(c,"pending");
            addReplyLongLong(c,raxSize(consumer->pel));
            addReplyStatus(c,"idle");
            addReplyLongLong(c,idle);
        }
        raxStop(&ri);
    } else if (!strcasecmp(opt,"GROUPS") && c->argc == 3) {
        /* XINFO GROUPS <key>. */
        if (s->cgroups == NULL) {
            addReplyMultiBulkLen(c,0);
            return;
        }

        addReplyMultiBulkLen(c,raxSize(s->cgroups));
        raxIterator ri;
        raxStart(&ri,s->cgroups);
        raxSeek(&ri,"^",NULL,0);
        while(raxNext(&ri)) {
            streamCG *cg = ri.data;
            addReplyMultiBulkLen(c,6);
            addReplyStatus(c,"name");
            addReplyBulkCBuffer(c,ri.key,ri.key_len);
            addReplyStatus(c,"consumers");
            addReplyLongLong(c,raxSize(cg->consumers));
            addReplyStatus(c,"pending");
            addReplyLongLong(c,raxSize(cg->pel));
        }
        raxStop(&ri);
    } else if (!strcasecmp(opt,"STREAM") && c->argc == 3) {
        /* XINFO STREAM <key> (or the alias XINFO <key>). */
        addReplyMultiBulkLen(c,12);
        addReplyStatus(c,"length");
        addReplyLongLong(c,s->length);
        addReplyStatus(c,"radix-tree-keys");
        addReplyLongLong(c,raxSize(s->rax));
        addReplyStatus(c,"radix-tree-nodes");
        addReplyLongLong(c,s->rax->numnodes);
        addReplyStatus(c,"groups");
        addReplyLongLong(c,s->cgroups ? raxSize(s->cgroups) : 0);

        /* To emit the first/last entry we us the streamReplyWithRange()
         * API. */
        int count;
        streamID start, end;
        start.ms = start.seq = 0;
        end.ms = end.seq = UINT64_MAX;
        addReplyStatus(c,"first-entry");
        count = streamReplyWithRange(c,s,&start,&end,1,0,NULL,NULL,
                                     STREAM_RWR_RAWENTRIES,NULL);
        if (!count) addReply(c,shared.nullbulk);
        addReplyStatus(c,"last-entry");
        count = streamReplyWithRange(c,s,&start,&end,1,1,NULL,NULL,
                                     STREAM_RWR_RAWENTRIES,NULL);
        if (!count) addReply(c,shared.nullbulk);
    } else {
        addReplyError(c,"syntax error, try 'XINFO HELP'");
    }
}

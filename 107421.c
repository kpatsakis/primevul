void streamPropagateXCLAIM(client *c, robj *key, robj *group, robj *id, streamNACK *nack) {
    /* We need to generate an XCLAIM that will work in a idempotent fashion:
     *
     * XCLAIM <key> <group> <consumer> 0 <id> TIME <milliseconds-unix-time>
     *        RETRYCOUNT <count> FORCE JUSTID.
     *
     * Note that JUSTID is useful in order to avoid that XCLAIM will do
     * useless work in the slave side, trying to fetch the stream item. */
    robj *argv[12];
    argv[0] = createStringObject("XCLAIM",6);
    argv[1] = key;
    argv[2] = group;
    argv[3] = createStringObject(nack->consumer->name,sdslen(nack->consumer->name));
    argv[4] = createStringObjectFromLongLong(0);
    argv[5] = id;
    argv[6] = createStringObject("TIME",4);
    argv[7] = createStringObjectFromLongLong(nack->delivery_time);
    argv[8] = createStringObject("RETRYCOUNT",10);
    argv[9] = createStringObjectFromLongLong(nack->delivery_count);
    argv[10] = createStringObject("FORCE",5);
    argv[11] = createStringObject("JUSTID",6);
    propagate(server.xclaimCommand,c->db->id,argv,12,PROPAGATE_AOF|PROPAGATE_REPL);
    decrRefCount(argv[0]);
    decrRefCount(argv[3]);
    decrRefCount(argv[4]);
    decrRefCount(argv[6]);
    decrRefCount(argv[7]);
    decrRefCount(argv[8]);
    decrRefCount(argv[9]);
    decrRefCount(argv[10]);
    decrRefCount(argv[11]);
}

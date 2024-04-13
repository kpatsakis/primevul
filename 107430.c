void xdelCommand(client *c) {
    robj *o;

    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.czero)) == NULL
        || checkType(c,o,OBJ_STREAM)) return;
    stream *s = o->ptr;

    /* We need to sanity check the IDs passed to start. Even if not
     * a big issue, it is not great that the command is only partially
     * executed becuase at some point an invalid ID is parsed. */
    streamID id;
    for (int j = 2; j < c->argc; j++) {
        if (streamParseIDOrReply(c,c->argv[j],&id,0) != C_OK) return;
    }

    /* Actaully apply the command. */
    int deleted = 0;
    for (int j = 2; j < c->argc; j++) {
        streamParseIDOrReply(c,c->argv[j],&id,0); /* Retval already checked. */
        deleted += streamDeleteItem(s,&id);
    }
    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(NOTIFY_STREAM,"xdel",c->argv[1],c->db->id);
    server.dirty += deleted;
    addReplyLongLong(c,deleted);
}

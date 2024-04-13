void *dupClientReplyValue(void *o) {
    incrRefCount((robj*)o);
    return o;
}

robj *createObjectFromStreamID(streamID *id) {
    return createObject(OBJ_STRING, sdscatfmt(sdsempty(),"%U-%U",
                        id->ms,id->seq));
}

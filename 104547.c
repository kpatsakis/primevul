static void v9fs_version(void *opaque)
{
    ssize_t err;
    V9fsPDU *pdu = opaque;
    V9fsState *s = pdu->s;
    V9fsString version;
    size_t offset = 7;

    v9fs_string_init(&version);
    err = pdu_unmarshal(pdu, offset, "ds", &s->msize, &version);
    if (err < 0) {
        offset = err;
        goto out;
    }
    trace_v9fs_version(pdu->tag, pdu->id, s->msize, version.data);

    virtfs_reset(pdu);

    if (!strcmp(version.data, "9P2000.u")) {
        s->proto_version = V9FS_PROTO_2000U;
    } else if (!strcmp(version.data, "9P2000.L")) {
        s->proto_version = V9FS_PROTO_2000L;
    } else {
        v9fs_string_sprintf(&version, "unknown");
    }

    err = pdu_marshal(pdu, offset, "ds", s->msize, &version);
    if (err < 0) {
        offset = err;
        goto out;
    }
    offset += err;
    trace_v9fs_version_return(pdu->tag, pdu->id, s->msize, version.data);
out:
    pdu_complete(pdu, offset);
    v9fs_string_free(&version);
}

static void v9fs_fsync(void *opaque)
{
    int err;
    int32_t fid;
    int datasync;
    size_t offset = 7;
    V9fsFidState *fidp;
    V9fsPDU *pdu = opaque;

    err = pdu_unmarshal(pdu, offset, "dd", &fid, &datasync);
    if (err < 0) {
        goto out_nofid;
    }
    trace_v9fs_fsync(pdu->tag, pdu->id, fid, datasync);

    fidp = get_fid(pdu, fid);
    if (fidp == NULL) {
        err = -ENOENT;
        goto out_nofid;
    }
    err = v9fs_co_fsync(pdu, fidp, datasync);
    if (!err) {
        err = offset;
    }
    put_fid(pdu, fidp);
out_nofid:
    pdu_complete(pdu, err);
}

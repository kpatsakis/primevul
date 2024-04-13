static void v9fs_readdir(void *opaque)
{
    int32_t fid;
    V9fsFidState *fidp;
    ssize_t retval = 0;
    size_t offset = 7;
    uint64_t initial_offset;
    int32_t count;
    uint32_t max_count;
    V9fsPDU *pdu = opaque;

    retval = pdu_unmarshal(pdu, offset, "dqd", &fid,
                           &initial_offset, &max_count);
    if (retval < 0) {
        goto out_nofid;
    }
    trace_v9fs_readdir(pdu->tag, pdu->id, fid, initial_offset, max_count);

    fidp = get_fid(pdu, fid);
    if (fidp == NULL) {
        retval = -EINVAL;
        goto out_nofid;
    }
    if (!fidp->fs.dir.stream) {
        retval = -EINVAL;
        goto out;
    }
    if (initial_offset == 0) {
        v9fs_co_rewinddir(pdu, fidp);
    } else {
        v9fs_co_seekdir(pdu, fidp, initial_offset);
    }
    count = v9fs_do_readdir(pdu, fidp, max_count);
    if (count < 0) {
        retval = count;
        goto out;
    }
    retval = pdu_marshal(pdu, offset, "d", count);
    if (retval < 0) {
        goto out;
    }
    retval += count + offset;
    trace_v9fs_readdir_return(pdu->tag, pdu->id, count, retval);
out:
    put_fid(pdu, fidp);
out_nofid:
    pdu_complete(pdu, retval);
}

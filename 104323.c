static int v9fs_xattr_read(V9fsState *s, V9fsPDU *pdu, V9fsFidState *fidp,
                           uint64_t off, uint32_t max_count)
{
    ssize_t err;
    size_t offset = 7;
    uint64_t read_count;
    V9fsVirtioState *v = container_of(s, V9fsVirtioState, state);
    VirtQueueElement *elem = v->elems[pdu->idx];

    if (fidp->fs.xattr.len < off) {
        read_count = 0;
    } else {
        read_count = fidp->fs.xattr.len - off;
    }
    if (read_count > max_count) {
        read_count = max_count;
    }
    err = pdu_marshal(pdu, offset, "d", read_count);
    if (err < 0) {
        return err;
    }
    offset += err;

    err = v9fs_pack(elem->in_sg, elem->in_num, offset,
                    ((char *)fidp->fs.xattr.value) + off,
                    read_count);
    if (err < 0) {
        return err;
    }
    offset += err;
    return offset;
}

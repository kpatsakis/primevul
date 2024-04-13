int64_t qcow2_alloc_bytes(BlockDriverState *bs, int size)
{
    BDRVQcowState *s = bs->opaque;
    int64_t offset, cluster_offset;
    int free_in_cluster;

    BLKDBG_EVENT(bs->file, BLKDBG_CLUSTER_ALLOC_BYTES);
    assert(size > 0 && size <= s->cluster_size);
    if (s->free_byte_offset == 0) {
        offset = qcow2_alloc_clusters(bs, s->cluster_size);
        if (offset < 0) {
            return offset;
        }
        s->free_byte_offset = offset;
    }
 redo:
    free_in_cluster = s->cluster_size -
        offset_into_cluster(s, s->free_byte_offset);
    if (size <= free_in_cluster) {
        /* enough space in current cluster */
        offset = s->free_byte_offset;
        s->free_byte_offset += size;
        free_in_cluster -= size;
        if (free_in_cluster == 0)
            s->free_byte_offset = 0;
        if (offset_into_cluster(s, offset) != 0)
            qcow2_update_cluster_refcount(bs, offset >> s->cluster_bits, 1,
                                          QCOW2_DISCARD_NEVER);
    } else {
        offset = qcow2_alloc_clusters(bs, s->cluster_size);
        if (offset < 0) {
            return offset;
        }
        cluster_offset = start_of_cluster(s, s->free_byte_offset);
        if ((cluster_offset + s->cluster_size) == offset) {
            /* we are lucky: contiguous data */
            offset = s->free_byte_offset;
            qcow2_update_cluster_refcount(bs, offset >> s->cluster_bits, 1,
                                          QCOW2_DISCARD_NEVER);
            s->free_byte_offset += size;
        } else {
            s->free_byte_offset = offset;
            goto redo;
        }
    }

    /* The cluster refcount was incremented, either by qcow2_alloc_clusters()
     * or explicitly by qcow2_update_cluster_refcount().  Refcount blocks must
     * be flushed before the caller's L2 table updates.
     */
    qcow2_cache_set_dependency(bs, s->l2_table_cache, s->refcount_block_cache);
    return offset;
}

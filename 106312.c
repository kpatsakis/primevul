static int QEMU_WARN_UNUSED_RESULT update_refcount(BlockDriverState *bs,
    int64_t offset, int64_t length, int addend, enum qcow2_discard_type type)
{
    BDRVQcowState *s = bs->opaque;
    int64_t start, last, cluster_offset;
    uint16_t *refcount_block = NULL;
    int64_t old_table_index = -1;
    int ret;

#ifdef DEBUG_ALLOC2
    fprintf(stderr, "update_refcount: offset=%" PRId64 " size=%" PRId64 " addend=%d\n",
           offset, length, addend);
#endif
    if (length < 0) {
        return -EINVAL;
    } else if (length == 0) {
        return 0;
    }

    if (addend < 0) {
        qcow2_cache_set_dependency(bs, s->refcount_block_cache,
            s->l2_table_cache);
    }

    start = start_of_cluster(s, offset);
    last = start_of_cluster(s, offset + length - 1);
    for(cluster_offset = start; cluster_offset <= last;
        cluster_offset += s->cluster_size)
    {
        int block_index, refcount;
        int64_t cluster_index = cluster_offset >> s->cluster_bits;
        int64_t table_index =
            cluster_index >> (s->cluster_bits - REFCOUNT_SHIFT);

        /* Load the refcount block and allocate it if needed */
        if (table_index != old_table_index) {
            if (refcount_block) {
                ret = qcow2_cache_put(bs, s->refcount_block_cache,
                    (void**) &refcount_block);
                if (ret < 0) {
                    goto fail;
                }
            }

            ret = alloc_refcount_block(bs, cluster_index, &refcount_block);
            if (ret < 0) {
                goto fail;
            }
        }
        old_table_index = table_index;

        qcow2_cache_entry_mark_dirty(s->refcount_block_cache, refcount_block);

        /* we can update the count and save it */
        block_index = cluster_index &
            ((1 << (s->cluster_bits - REFCOUNT_SHIFT)) - 1);

        refcount = be16_to_cpu(refcount_block[block_index]);
        refcount += addend;
        if (refcount < 0 || refcount > 0xffff) {
            ret = -EINVAL;
            goto fail;
        }
        if (refcount == 0 && cluster_index < s->free_cluster_index) {
            s->free_cluster_index = cluster_index;
        }
        refcount_block[block_index] = cpu_to_be16(refcount);

        if (refcount == 0 && s->discard_passthrough[type]) {
            update_refcount_discard(bs, cluster_offset, s->cluster_size);
        }
    }

    ret = 0;
fail:
    if (!s->cache_discards) {
        qcow2_process_discards(bs, ret);
    }

    /* Write last changed block to disk */
    if (refcount_block) {
        int wret;
        wret = qcow2_cache_put(bs, s->refcount_block_cache,
            (void**) &refcount_block);
        if (wret < 0) {
            return ret < 0 ? ret : wret;
        }
    }

    /*
     * Try do undo any updates if an error is returned (This may succeed in
     * some cases like ENOSPC for allocating a new refcount block)
     */
    if (ret < 0) {
        int dummy;
        dummy = update_refcount(bs, offset, cluster_offset - offset, -addend,
                                QCOW2_DISCARD_NEVER);
        (void)dummy;
    }

    return ret;
}

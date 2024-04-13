static int64_t realloc_refcount_block(BlockDriverState *bs, int reftable_index,
                                      uint64_t offset)
{
    BDRVQcowState *s = bs->opaque;
    int64_t new_offset = 0;
    void *refcount_block = NULL;
    int ret;

    /* allocate new refcount block */
    new_offset = qcow2_alloc_clusters(bs, s->cluster_size);
    if (new_offset < 0) {
        fprintf(stderr, "Could not allocate new cluster: %s\n",
                strerror(-new_offset));
        ret = new_offset;
        goto done;
    }

    /* fetch current refcount block content */
    ret = qcow2_cache_get(bs, s->refcount_block_cache, offset, &refcount_block);
    if (ret < 0) {
        fprintf(stderr, "Could not fetch refcount block: %s\n", strerror(-ret));
        goto fail_free_cluster;
    }

    /* new block has not yet been entered into refcount table, therefore it is
     * no refcount block yet (regarding this check) */
    ret = qcow2_pre_write_overlap_check(bs, 0, new_offset, s->cluster_size);
    if (ret < 0) {
        fprintf(stderr, "Could not write refcount block; metadata overlap "
                "check failed: %s\n", strerror(-ret));
        /* the image will be marked corrupt, so don't even attempt on freeing
         * the cluster */
        goto done;
    }

    /* write to new block */
    ret = bdrv_write(bs->file, new_offset / BDRV_SECTOR_SIZE, refcount_block,
            s->cluster_sectors);
    if (ret < 0) {
        fprintf(stderr, "Could not write refcount block: %s\n", strerror(-ret));
        goto fail_free_cluster;
    }

    /* update refcount table */
    assert(!offset_into_cluster(s, new_offset));
    s->refcount_table[reftable_index] = new_offset;
    ret = write_reftable_entry(bs, reftable_index);
    if (ret < 0) {
        fprintf(stderr, "Could not update refcount table: %s\n",
                strerror(-ret));
        goto fail_free_cluster;
    }

    goto done;

fail_free_cluster:
    qcow2_free_clusters(bs, new_offset, s->cluster_size, QCOW2_DISCARD_OTHER);

done:
    if (refcount_block) {
        /* This should never fail, as it would only do so if the given refcount
         * block cannot be found in the cache. As this is impossible as long as
         * there are no bugs, assert the success. */
        int tmp = qcow2_cache_put(bs, s->refcount_block_cache, &refcount_block);
        assert(tmp == 0);
    }

    if (ret < 0) {
        return ret;
    }

    return new_offset;
}

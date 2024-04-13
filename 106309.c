int qcow2_update_cluster_refcount(BlockDriverState *bs,
                                  int64_t cluster_index,
                                  int addend,
                                  enum qcow2_discard_type type)
{
    BDRVQcowState *s = bs->opaque;
    int ret;

    ret = update_refcount(bs, cluster_index << s->cluster_bits, 1, addend,
                          type);
    if (ret < 0) {
        return ret;
    }

    return get_refcount(bs, cluster_index);
}

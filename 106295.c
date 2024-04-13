static int in_same_refcount_block(BDRVQcowState *s, uint64_t offset_a,
    uint64_t offset_b)
{
    uint64_t block_a = offset_a >> (2 * s->cluster_bits - REFCOUNT_SHIFT);
    uint64_t block_b = offset_b >> (2 * s->cluster_bits - REFCOUNT_SHIFT);

    return (block_a == block_b);
}

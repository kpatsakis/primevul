static inline int flexarray_get_bin_size(struct flexarray *flex, int bin)
{
    (void)flex;
    if (bin >= ARRAY_SIZE(bin_offset))
        return -1;
    int next = bin_offset[bin + 1];
    return next - bin_offset[bin];
}

static inline int flexarray_get_bin_offset(struct flexarray *flex, int bin, int index)
{
    (void)flex;
    return index - bin_offset[bin];
}

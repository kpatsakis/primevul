static inline void *flexarray_get(struct flexarray *flex, int index)
{
    int bin;

    if (index >= flex->item_count)
        return NULL;
    bin = flexarray_get_bin(flex, index);
    if (bin < 0 || bin >= flex->bin_count)
        return NULL;
    return flex->bins[bin][flexarray_get_bin_offset(flex, bin, index)];
}

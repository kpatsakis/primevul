static int flexarray_set(struct flexarray *flex, int index, void *data)
{
    int bin = flexarray_get_bin(flex, index);
    if (bin < 0)
        return -EINVAL;
    if (bin >= flex->bin_count) {
        void *bins = realloc(flex->bins, (flex->bin_count + 1) *
                             sizeof(flex->bins));
        if (!bins)
            return -ENOMEM;
        flex->bin_count++;
        flex->bins = bins;
        flex->bins[flex->bin_count - 1] =
            calloc(flexarray_get_bin_size(flex, flex->bin_count - 1),
                   sizeof(void *));
        if (!flex->bins[flex->bin_count - 1]) {
            flex->bin_count--;
            return -ENOMEM;
        }
    }
    flex->item_count++;
    flex->bins[bin][flexarray_get_bin_offset(flex, bin, index)] = data;
    return flex->item_count - 1;
}

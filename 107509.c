static inline int flexarray_get_bin(struct flexarray *flex, int index)
{
    int i;
    (void)flex;
    for (i = 0; i < ARRAY_SIZE(bin_offset); i++)
        if (index < bin_offset[i])
            return i - 1;
    return -1;
}

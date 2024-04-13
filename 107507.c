static void flexarray_clear(struct flexarray *flex)
{
    int i;
    for (i = 0; i < flex->bin_count; i++)
        free(flex->bins[i]);
    free(flex->bins);
    flex->bin_count = 0;
    flex->item_count = 0;
}

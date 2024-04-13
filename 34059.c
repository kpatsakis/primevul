pdf14_get_cmap_procs(const gs_gstate *pgs, const gx_device * dev)
{
    /* The pdf14 marking device itself is always continuous tone. */
    return &pdf14_cmap_many;
}

pdf14_device_finalize(const gs_memory_t *cmem, void *vptr)
{
    gx_device * const dev = (gx_device *)vptr;
    pdf14_device * pdev = (pdf14_device *)dev;

    pdf14_cleanup_parent_color_profiles (pdev);

    if (pdev->ctx) {
        pdf14_ctx_free(pdev->ctx);
        pdev->ctx = NULL;
    }

    while (pdev->trans_group_parent_cmap_procs) {
        pdf14_pop_parent_color(dev, NULL);
    }
    gx_device_finalize(cmem, vptr);
}

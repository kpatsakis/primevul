pdf14_clist_begin_image(gx_device * dev,
                       const gs_gstate * pgs, const gs_image_t * pim,
                       gs_image_format_t format, const gs_int_rect * prect,
                       const gx_drawing_color * pdcolor,
                       const gx_clip_path * pcpath,
                       gs_memory_t * memory, gx_image_enum_common_t ** pinfo)
{
    pdf14_clist_device * pdev = (pdf14_clist_device *)dev;
    int code;

    /*
     * Ensure that that the PDF 1.4 reading compositor will have the current
     * blending parameters.  This is needed since the fill_rectangle routines
     * do not have access to the gs_gstate.  Thus we have to pass any
     * changes explictly.
     */
    code = pdf14_clist_update_params(pdev, pgs, false, NULL);
    if (code < 0)
        return code;
    /* Pass image to the target */
    code = gx_forward_begin_image(dev, pgs, pim, format, prect,
                                        pdcolor, pcpath, memory, pinfo);
    if (code < 0)
        return gx_default_begin_image(dev, pgs, pim, format, prect,
                                        pdcolor, pcpath, memory, pinfo);
    else return code;

}

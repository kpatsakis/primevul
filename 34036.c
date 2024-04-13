pdf14_clist_stroke_path(gx_device *dev,	const gs_gstate *pgs,
                             gx_path *ppath, const gx_stroke_params *params,
                             const gx_drawing_color *pdcolor,
                             const gx_clip_path *pcpath)
{
    pdf14_clist_device * pdev = (pdf14_clist_device *)dev;
    gs_gstate new_pgs = *pgs;
    int code;
    gs_pattern2_instance_t *pinst = NULL;

    /*
     * Ensure that that the PDF 1.4 reading compositor will have the current
     * blending parameters.  This is needed since the fill_rectangle routines
     * do not have access to the gs_gstate.  Thus we have to pass any
     * changes explictly.
     */
    code = pdf14_clist_update_params(pdev, pgs, false, NULL);
    if (code < 0)
        return code;
    /* If we are doing a shading stroke and we are in a transparency group of a
       different color space, then we need to get the proper device information
       passed along so that we use the correct color procs and colorinfo about
       the transparency device and not the final target device */
    if (pdcolor != NULL && gx_dc_is_pattern2_color(pdcolor) &&
        pdev->trans_group_parent_cmap_procs != NULL) {
        pinst =
            (gs_pattern2_instance_t *)pdcolor->ccolor.pattern;
           pinst->saved->has_transparency = true;
           /* The transparency color space operations are driven
              by the pdf14 clist writer device.  */
           pinst->saved->trans_device = dev;
    }

    update_lop_for_pdf14(&new_pgs, pdcolor);
    new_pgs.trans_device = dev;
    new_pgs.has_transparency = true;
    code = gx_forward_stroke_path(dev, &new_pgs, ppath, params, pdcolor, pcpath);
    new_pgs.trans_device = NULL;
    new_pgs.has_transparency = false;
    if (pinst != NULL){
        pinst->saved->trans_device = NULL;
    }
    return code;
}

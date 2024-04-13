pdf14_cmap_cmyk_direct(frac c, frac m, frac y, frac k, gx_device_color * pdc,
     const gs_gstate * pgs, gx_device * dev, gs_color_select_t select,
     const gs_color_space *pcs)
{
    int i,ncomps;
    frac cm_comps[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_value cv[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_index color;
    gx_device *trans_device;

    /* If trans device is set, we need to use its procs. */
    if (pgs->trans_device != NULL){
        trans_device = pgs->trans_device;
    } else {
        trans_device = dev;
    }
    ncomps = trans_device->color_info.num_components;
    /* map to the color model */
    dev_proc(trans_device, get_color_mapping_procs)(trans_device)->map_cmyk(trans_device, c, m, y, k, cm_comps);
    for (i = 0; i < ncomps; i++)
        cv[i] = frac2cv(gx_map_color_frac(pgs, cm_comps[i], effective_transfer[i]));
    /* if output device supports devn, we need to make sure we send it the
       proper color type */
    if (dev_proc(trans_device, dev_spec_op)(trans_device, gxdso_supports_devn, NULL, 0)) {
        for (i = 0; i < ncomps; i++)
            pdc->colors.devn.values[i] = cv[i];
        pdc->type = gx_dc_type_devn;
    } else {
    /* encode as a color index */
        color = dev_proc(trans_device, encode_color)(trans_device, cv);
        /* check if the encoding was successful; we presume failure is rare */
        if (color != gx_no_color_index)
            color_set_pure(pdc, color);
    }
}

pdf14_cmap_devicen_direct(const	frac * pcc,
    gx_device_color * pdc, const gs_gstate * pgs, gx_device * dev,
    gs_color_select_t select)
{
    int i, ncomps = dev->color_info.num_components;
    int num_spots = pdf14_get_num_spots(dev);
    frac cm_comps[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_value cv[GX_DEVICE_COLOR_MAX_COMPONENTS];
    gx_color_index color;
    gx_device *trans_device;

     /*  We may be coming from the clist writer which often forwards us the
         target device. If this occurs we actually need to get to the color
         space defined by the transparency group and we use the operators
         defined by the transparency device to do the job.
       */
    if (pgs->trans_device != NULL){
        trans_device = pgs->trans_device;
    } else {
        trans_device = dev;
    }
    ncomps = trans_device->color_info.num_components;
    /* map to the color model */
    map_components_to_colorants(pcc, &(pgs->color_component_map), cm_comps);;
    /* apply the transfer function(s); convert to color values */
    if (trans_device->color_info.polarity == GX_CINFO_POLARITY_ADDITIVE) {
        for (i = 0; i < ncomps; i++)
            cv[i] = frac2cv(gx_map_color_frac(pgs, cm_comps[i], effective_transfer[i]));
        /* We are in an additive mode (blend space) and drawing with a sep color
        into a sep device.  Make sure we are drawing "white" with the process
        colorants */
        for (i = 0; i < ncomps - num_spots; i++)
            cv[i] = gx_max_color_value;
    } else
        for (i = 0; i < ncomps; i++)
            cv[i] = frac2cv(frac_1 - gx_map_color_frac(pgs, (frac)(frac_1 - cm_comps[i]), effective_transfer[i]));
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

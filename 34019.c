get_pdf14_clist_device_proto(gx_device * dev, pdf14_clist_device ** pdevproto,
        pdf14_clist_device * ptempdevproto, gs_gstate * pgs,
        const gs_pdf14trans_t * pdf14pct, bool use_pdf14_accum)
{
    bool using_blend_cs;
    pdf14_default_colorspace_t dev_cs =
                pdf14_determine_default_blend_cs(dev, use_pdf14_accum,
                                                 &using_blend_cs);

    switch (dev_cs) {
        case PDF14_DeviceGray:
            *pdevproto = (pdf14_clist_device *)&pdf14_clist_Gray_device;
           /* We want gray to be single channel.  Low level
               initialization of gray device prototype is
               peculiar in that in dci_std_color_num_components
               the comment is
              "A device is monochrome only if it is bi-level"
              Here we want monochrome anytime we have a gray device.
              To avoid breaking things elsewhere, we will overide
              the prototype intialization here */
            *ptempdevproto = **pdevproto;
            ptempdevproto->color_info.max_components = 1;
            ptempdevproto->color_info.num_components =
                                    ptempdevproto->color_info.max_components;
            ptempdevproto->color_info.max_gray = 255;
            ptempdevproto->color_info.gray_index = 0; /* Avoid halftoning */
            ptempdevproto->color_info.dither_grays = 256;
            ptempdevproto->color_info.anti_alias = dev->color_info.anti_alias;
            ptempdevproto->sep_device = false;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceRGB:
            *pdevproto = (pdf14_clist_device *)&pdf14_clist_RGB_device;
            *ptempdevproto = **pdevproto;
            ptempdevproto->color_info.anti_alias = dev->color_info.anti_alias;
            ptempdevproto->sep_device = false;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceCMYK:
            *pdevproto = (pdf14_clist_device *)&pdf14_clist_CMYK_device;
            *ptempdevproto = **pdevproto;
            ptempdevproto->color_info.anti_alias = dev->color_info.anti_alias;
            ptempdevproto->sep_device = false;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceCMYKspot:
            *pdevproto = (pdf14_clist_device *)&pdf14_clist_CMYKspot_device;
            *ptempdevproto = **pdevproto;
            /*
             * The number of components for the PDF14 device is the sum
             * of the process components and the number of spot colors
             * for the page.
             */
            if (pdf14pct->params.num_spot_colors >= 0) {
                ptempdevproto->devn_params.page_spot_colors =
                    pdf14pct->params.num_spot_colors;
                ptempdevproto->color_info.num_components =
                    ptempdevproto->devn_params.num_std_colorant_names +
                    pdf14pct->params.num_spot_colors;
                if (ptempdevproto->color_info.num_components >
                        ptempdevproto->color_info.max_components)
                    ptempdevproto->color_info.num_components =
                        ptempdevproto->color_info.max_components;
                ptempdevproto->color_info.depth =
                                    ptempdevproto->color_info.num_components * 8;
            }
            ptempdevproto->color_info.anti_alias = dev->color_info.anti_alias;
            ptempdevproto->sep_device = true;
            *pdevproto = ptempdevproto;
            break;
        case PDF14_DeviceCustom:
            /*
             * We are using the output device's process color model.  The
             * color_info for the PDF 1.4 compositing device needs to match
             * the output device.
             */
            *ptempdevproto = pdf14_clist_custom_device;
            ptempdevproto->color_info = dev->color_info;
            /* The pdf14 device has to be 8 bit continuous tone. Force it */
            ptempdevproto->color_info.depth =
                ptempdevproto->color_info.num_components * 8;
            ptempdevproto->color_info.max_gray = 255;
            ptempdevproto->color_info.max_color = 255;
            ptempdevproto->color_info.dither_grays = 256;
            ptempdevproto->color_info.dither_colors = 256;
            ptempdevproto->color_info.anti_alias = dev->color_info.anti_alias;
            *pdevproto = ptempdevproto;
            break;
        default:			/* Should not occur */
            return_error(gs_error_rangecheck);
    }
    ptempdevproto->using_blend_cs = using_blend_cs;
    return 0;
}

put_param_pdf14_spot_names(gx_device * pdev,
                gs_separations * pseparations, gs_param_list * plist)
{
    int code, num_spot_colors, i;
    gs_param_string str;

    /* Check if the given keyname is present. */
    code = param_read_int(plist, PDF14NumSpotColorsParamName,
                                                &num_spot_colors);
    switch (code) {
        default:
            param_signal_error(plist, PDF14NumSpotColorsParamName, code);
            break;
        case 1:
            return 0;
        case 0:
            if (num_spot_colors < 1 ||
                num_spot_colors > GX_DEVICE_COLOR_MAX_COMPONENTS)
                return_error(gs_error_rangecheck);
            for (i = 0; i < num_spot_colors; i++) {
                char buff[20];
                byte * sep_name;

                gs_sprintf(buff, "PDF14SpotName_%d", i);
                code = param_read_string(plist, buff, &str);
                switch (code) {
                    default:
                        param_signal_error(plist, buff, code);
                        break;
                    case 0:
                        sep_name = gs_alloc_bytes(pdev->memory,
                                str.size, "put_param_pdf14_spot_names");
                        memcpy(sep_name, str.data, str.size);
                        pseparations->names[i].size = str.size;
                        pseparations->names[i].data = sep_name;
                }
            }
            pseparations->num_separations = num_spot_colors;
            break;
    }
    return 0;;
}

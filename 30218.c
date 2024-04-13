static int cmykbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;
    float CMYK[4], Gray, RGB[3];
    int i;
    const gs_color_space *  pcs = gs_currentcolorspace(igs);

    if (pcs->id == cs_DeviceGray_id) {
        /* UGLY hack. Its possible for the graphics library to change the
         * colour space to DeviceGray (setcachedevice), but this does not
         * change the PostScript space. It can't, because the graphics library
         * doesn't know about the PostScript objects. If we get a current*
         * operation before the space has been restored, the colour space in
         * the graphics library and the PostScript stored space won't match.
         * If that happens then we need to pretend the PS colour space was
         * DeviceGray
         */
        return(graybasecolor(i_ctx_p, space, base, stage, cont, stack_depth));
    }

    *cont = 0;
    *stage = 0;
    check_op(4);
    op -= 3;
    for (i=0;i<4;i++) {
        if (!r_has_type(op, t_integer)) {
            if (r_has_type(op, t_real)) {
                CMYK[i] = op->value.realval;
            } else
                return_error(gs_error_typecheck);
        } else
            CMYK[i] = (float)op->value.intval;
        if (CMYK[i] < 0 || CMYK[i] > 1)
            return_error(gs_error_rangecheck);
        op++;
    }

    switch (base) {
        case 0:
            pop(3);
            op = osp;
            Gray = (0.3 * CMYK[0]) + (0.59 * CMYK[1]) + (0.11 * CMYK[2]) + CMYK[3];
            if (Gray > 1.0)
                Gray = 0;
            else
                Gray = 1.0 - Gray;
            make_real(op, Gray);
            break;
        case 1:
        case 2:
            pop(1);
            op = osp;
            RGB[0] = 1.0 - (CMYK[0] + CMYK[3]);
            if (RGB[0] < 0)
                RGB[0] = 0;
            RGB[1] = 1.0 - (CMYK[1] + CMYK[3]);
            if (RGB[1] < 0)
                RGB[1] = 0;
            RGB[2] = 1.0 - (CMYK[2] + CMYK[3]);
            if (RGB[2] < 0)
                RGB[2] = 0;
            if (base == 1)
                rgb2hsb((float *)&RGB);
            make_real(&op[-2], RGB[0]);
            make_real(&op[-1], RGB[1]);
            make_real(op, RGB[2]);
            break;
        case 3:
            op = osp;
            make_real(&op[-3], CMYK[0]);
            make_real(&op[-2], CMYK[1]);
            make_real(&op[-1], CMYK[2]);
            make_real(op, CMYK[3]);
            break;
        default:
            return_error(gs_error_undefined);
    }
    return 0;
}

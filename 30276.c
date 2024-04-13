static int rgbbasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op = osp;
    float RGB[3], CMYK[4], Gray, UCR, BG;
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

    switch (*stage) {
        case 0:
            *cont = 0;
            check_op(3);
            op -= 2;
            for (i=0;i<3;i++) {
                if (!r_has_type(op, t_integer)) {
                    if (r_has_type(op, t_real)) {
                        RGB[i] = op->value.realval;
                    } else
                        return_error(gs_error_typecheck);
                } else
                    RGB[i] = (float)op->value.intval;
                if (RGB[i] < 0 || RGB[i] > 1)
                    return_error(gs_error_rangecheck);
                op++;
            }
            op = osp;

            switch (base) {
                case 0:
                    pop(2);
                    op = osp;
                    /* If R == G == B, then this is gray, so just use it. Avoids
                     * rounding errors.
                     */
                    if (RGB[0] == RGB[1] && RGB[1] == RGB[2])
                        Gray = RGB[0];
                    else
                        Gray = (0.3 * RGB[0]) + (0.59 * RGB[1]) + (0.11 * RGB[2]);
                    make_real(op, Gray);
                    return 0;
                    break;
                case 1:
                    rgb2hsb((float *)&RGB);
                    make_real(&op[-2], RGB[0]);
                    make_real(&op[-1], RGB[1]);
                    make_real(op, RGB[2]);
                    return 0;
                    break;
                case 2:
                    make_real(&op[-2], RGB[0]);
                    make_real(&op[-1], RGB[1]);
                    make_real(op, RGB[2]);
                    return 0;
                    break;
                case 3:
                    *stage = 1;
                    *cont = 1;
                    for (i=0;i<3;i++)
                        CMYK[i] = 1 - RGB[i];
                    if (CMYK[0] < CMYK[1]) {
                        if (CMYK[0] < CMYK[2])
                            CMYK[3] = CMYK[0];
                        else
                            CMYK[3] = CMYK[2];
                    } else {
                        if (CMYK[1] < CMYK[2])
                            CMYK[3] = CMYK[1];
                        else
                            CMYK[3] = CMYK[2];
                    }
                    check_estack(1);
                    push(2);
                    op = osp - 4;
                    for (i=0;i<4;i++) {
                        make_real(op, CMYK[i]);
                        op++;
                    }
                    make_real(op, CMYK[3]);
                    esp++;
                    *esp = istate->undercolor_removal;
                    return o_push_estack;
                    break;
                default:
                    return_error(gs_error_undefined);
                    break;
            }
            break;
        case 1:
            (*stage)++;
            *cont = 1;
            check_estack(1);
            check_op(5);
            op -= 4;
            for (i=0;i<4;i++) {
                if (!r_has_type(op, t_integer)) {
                    if (r_has_type(op, t_real)) {
                        CMYK[i] = op->value.realval;
                    } else
                        return_error(gs_error_typecheck);
                } else
                    CMYK[i] = (float)op->value.intval;
                op++;
            }
            if (!r_has_type(op, t_integer)) {
                if (r_has_type(op, t_real)) {
                    UCR = op->value.realval;
                } else
                    return_error(gs_error_typecheck);
            } else
                UCR = (float)op->value.intval;
            for (i=0;i<3;i++) {
                CMYK[i] = CMYK[i] - UCR;
                if (CMYK[i] < 0)
                    CMYK[i] = 0;
                if (CMYK[i] > 1)
                    CMYK[i] = 1.0;
            }
            op -= 4;
            for (i=0;i<4;i++) {
                make_real(op, CMYK[i]);
                op++;
            }
            make_real(op, CMYK[3]);
            esp++;
            *esp = istate->black_generation;
            return o_push_estack;
            break;
        case 2:
            *stage = 0;
            *cont = 0;
            check_op(5);
            if (!r_has_type(op, t_integer)) {
                if (r_has_type(op, t_real)) {
                    BG = op->value.realval;
                } else
                    return_error(gs_error_typecheck);
            } else
                BG = (float)op->value.intval;
            pop(1);
            op = osp;
            if (BG < 0)
                BG = 0;
            if (BG > 1)
                BG = 1;
            make_real(op, BG);
            break;
    }
    return 0;
}

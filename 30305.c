static int setlabspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont,
                       int CIESubst)
{
    /* In this case, we will treat this as an ICC color space, with a
       CIELAB 16 bit profile */
    ref labdict;
    int code = 0;
    float                   range_buff[4], white[3], black[3];
    static const float      dflt_range[4] = { -100, 100, -100, 100 };
    static const float      dflt_black[3] = {0,0,0}, dflt_white[3] = {0,0,0};
    int i;
    gs_client_color cc;

    *cont = 0;
    code = array_get(imemory, r, 1, &labdict);
    if (code < 0)
        return code;
/* Get all the parts */
    code = dict_floats_param( imemory, &labdict, "Range", 4, range_buff,
                              dflt_range );
    if (code < 0)
        return code;
    for (i = 0; i < 4 && range_buff[i + 1] >= range_buff[i]; i += 2);
    if (i != 4)
        return_error(gs_error_rangecheck);
    code = dict_floats_param( imemory, &labdict, "BlackPoint", 3, black,
                              dflt_black );
    if (code < 0)
        return code;
    code = dict_floats_param( imemory, &labdict, "WhitePoint", 3, white,
                              dflt_white );
    if (code < 0)
        return code;
    if (white[0] <= 0 || white[1] != 1.0 || white[2] <= 0)
        return_error(gs_error_rangecheck);
    code = seticc_lab(i_ctx_p, white, black, range_buff);
    if ( code < 0)
        return gs_rethrow(code, "setting PDF lab color space");
    cc.pattern = 0x00;
    for (i=0;i<3;i++)
        cc.paint.values[i] = 0;
    code = gs_setcolor(igs, &cc);
    return code;
}

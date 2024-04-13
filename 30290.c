static int setcalgrayspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    ref graydict;
    int code = 0;
    float                   gamma, white[3], black[3];
    double                  dflt_gamma = 1.0;
    static const float      dflt_black[3] = {0,0,0}, dflt_white[3] = {0,0,0};
    gs_client_color cc;

    *cont = 0;
    code = array_get(imemory, r, 1, &graydict);
    if (code < 0)
        return code;
/* Get all the parts */
    code = dict_float_param(&graydict, "Gamma",
                 dflt_gamma, &gamma);
    if (code < 0)
        return code;
    if (gamma <= 0 )
        return_error(gs_error_rangecheck);
    code = dict_floats_param( imemory,
                              &graydict,
                              "BlackPoint",
                              3,
                              black,
                              dflt_black );
    if (code < 0)
        return code;
    code = dict_floats_param( imemory,
                              &graydict,
                              "WhitePoint",
                              3,
                              white,
                              dflt_white );
    if (code < 0)
        return code;
    if (white[0] <= 0 || white[1] != 1.0 || white[2] <= 0)
        return_error(gs_error_rangecheck);
    code = seticc_cal(i_ctx_p, white, black, &gamma, NULL, 1,
                        graydict.value.saveid);
    if ( code < 0)
        return gs_rethrow(code, "setting CalGray  color space");
    cc.pattern = 0x00;
    cc.paint.values[0] = 0;
    code = gs_setcolor(igs, &cc);
    return code;
}

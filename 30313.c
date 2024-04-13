static int validatecalrgbspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code=0;
    ref *space, calrgbdict;

    space = *r;
    if (!r_is_array(space))
        return_error(gs_error_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(space) < 2)
        return_error(gs_error_rangecheck);
    code = array_get(imemory, space, 1, &calrgbdict);
    if (code < 0)
        return code;
    if (!r_has_type(&calrgbdict, t_dictionary))
        return_error(gs_error_typecheck);
    /* Check the white point, which is required */
    code = checkWhitePoint(i_ctx_p, &calrgbdict);
    if (code != 0)
        return code;
    /* The rest are optional.  Need to validate though */
    code = checkBlackPoint(i_ctx_p, &calrgbdict);
    if (code < 0)
        return code;
    /* Check Gamma values */
    code = checkGamma(i_ctx_p, &calrgbdict, 3);
    if (code < 0)
        return code;
    /* Check Matrix */
    code = checkCalMatrix(i_ctx_p, &calrgbdict);
    if (code < 0)
        return code;
    *r = 0;  /* No nested space */
    return 0;
}

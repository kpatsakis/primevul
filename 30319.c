static int validatedevicepspace(i_ctx_t * i_ctx_p, ref **space)
{
    int code = 0;
    ref *r = *space, bpp;

    if (!r_is_array(r))
        return_error(gs_error_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(r) != 2)
        return_error(gs_error_rangecheck);
    /* Make sure 'bits per pixel' is an integer */
    code = array_get(imemory, r, 1, &bpp);
    if (code < 0)
        return code;
    if (!r_has_type(&bpp, t_integer))
        return_error(gs_error_typecheck);

    /* Make sure 'bits per pixel' lies between 0 and 31 */
    if (bpp.value.intval < 0 || bpp.value.intval > 31)
        return_error(gs_error_rangecheck);

    *space = 0;
    return code;
}

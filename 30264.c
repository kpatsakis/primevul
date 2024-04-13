static int indexedvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    int code, integer;
    float fraction;
    ref hival;
    os_ptr op = osp;

    if (num_comps < 1)
        return_error(gs_error_stackunderflow);

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
        return_error(gs_error_typecheck);

    code = array_get(imemory, space, 2, &hival);
    if (code < 0)
        return code;

    if (*values > hival.value.intval)
        *values = (float)hival.value.intval;

    if (*values < 0)
        *values = 0;

    /* The PLRM says 'If it is a real number, it is rounded to the nearest integer
     * but in fact Acrobat simply floors the value.
     *
     * KAS 29/08/2017 the comment above is incorrect, at least for recent versions of Acrobat.
     * In addition the PDF 2.0 specification states that real numbers should be rounded to
     * the nearest integer (0.5 rounds up) and if it is outside 0->hival then it should
     * be adjusted to the nearest value within that range (which is done above). The Quality
     * Logic 2.0 FTS includes a test for this which Acrobat X fails.
     */
    integer = (int)floor(*values);
    fraction = *values - integer;

    if (fraction >= 0.5)
        *values = (float)(integer + 1);
    else
        *values = (float)integer;

    return 0;
}

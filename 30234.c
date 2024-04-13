static int devicenvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    int i, code;
    ref narray;
    os_ptr op = osp;

    code = array_get(imemory, space, 1, &narray);
    if (code < 0)
        return code;
    if (!r_is_array(&narray))
        return_error(gs_error_typecheck);

    if (num_comps < r_size(&narray))
        return_error(gs_error_stackunderflow);

    op -= r_size(&narray) - 1;

    for (i=0;i < r_size(&narray); i++) {
        if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
            return_error(gs_error_typecheck);

        if (values[i] > 1.0)
            values[i] = 1.0;

        if (values[i] < 0.0)
            values[i] = 0.0;
        op++;
    }

    return 0;
}

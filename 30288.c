static int sepvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;

    if (num_comps < 1)
        return_error(gs_error_stackunderflow);

    if (!r_has_type(op, t_integer) && !r_has_type(op, t_real))
        return_error(gs_error_typecheck);

    if (*values > 1.0)
        *values = 1.0;

    if (*values < 0.0)
        *values = 0.0;

    return 0;
}

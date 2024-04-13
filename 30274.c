static int patternvalidate(i_ctx_t *i_ctx_p, ref *space, float *values, int num_comps)
{
    os_ptr op = osp;

    check_op(1);

    if (!r_has_type(op, t_dictionary) && !r_has_type(op, t_null))
        return_error(gs_error_typecheck);

    return 0;
}

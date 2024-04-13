static int patterncomponent(i_ctx_t * i_ctx_p, ref *space, int *n)
{
    os_ptr op = osp;
    int n_comps, code;
    const gs_color_space *  pcs = gs_currentcolorspace(igs);
    gs_client_color         cc;

    /* check for a pattern color space */
    if ((n_comps = cs_num_components(pcs)) < 0) {
        n_comps = -n_comps;
        if (r_has_type(op, t_dictionary)) {
            ref     *pImpl, pPatInst;

            code = dict_find_string(op, "Implementation", &pImpl);
        if (code > 0) {
            code = array_get(imemory, pImpl, 0, &pPatInst);
            if (code < 0)
                return code;
            cc.pattern = r_ptr(&pPatInst, gs_pattern_instance_t);
            if (pattern_instance_uses_base_space(cc.pattern))
                *n = n_comps;
            else
                *n = 1;
            } else
            *n = 1;
        } else
            *n = 1;
    } else
        return_error(gs_error_typecheck);

    return 0;
}

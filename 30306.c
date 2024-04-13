static int setpatternspace(i_ctx_t * i_ctx_p, ref *r, int *stage, int *cont, int CIESubst)
{
    gs_color_space *pcs;
    gs_color_space *pcs_base;
    uint edepth = ref_stack_count(&e_stack);
    int code = 0;

    if (i_ctx_p->language_level < 2)
        return_error(gs_error_undefined);

    *cont = 0;
    pcs_base = NULL;
    if (r_is_array(r)) {
        check_read(*r);

        switch (r_size(r)) {
            case 1:		/* no base space */
                pcs_base = NULL;
                break;
            default:
                return_error(gs_error_rangecheck);
            case 2:
                pcs_base = gs_currentcolorspace(igs);
                if (cs_num_components(pcs_base) < 0)       /* i.e., Pattern space */
                    return_error(gs_error_rangecheck);
        }
    }
    pcs = gs_cspace_alloc(imemory, &gs_color_space_type_Pattern);
    pcs->base_space = pcs_base;
    pcs->params.pattern.has_base_space = (pcs_base != NULL);
    rc_increment_cs(pcs_base);
    code = gs_setcolorspace(igs, pcs);
    /* release reference from construction */
    rc_decrement_only_cs(pcs, "zsetpatternspace");
    if (code < 0) {
        ref_stack_pop_to(&e_stack, edepth);
        return code;
    }
    make_null(&istate->pattern[0]); /* PLRM: initial color value is a null object */
    *stage = 0;
    return (ref_stack_count(&e_stack) == edepth ? 0 : o_push_estack);	/* installation will load the caches */
}

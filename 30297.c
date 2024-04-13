setcolorspace_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    os_ptr op = osp;
    es_ptr ep = esp, pdepth, pstage, pCIESubst;
    int i, code = 0, stage, cont, CIESubst = 0;
    unsigned int depth;
    PS_colour_space_t *obj;

    pCIESubst = &ep[-3];
    pdepth = &ep[-2];
    pstage = &ep[-1];

    CIESubst = (int)pCIESubst->value.intval;
    depth = (unsigned int)pdepth->value.intval;
    stage = (int)pstage->value.intval;
    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining stages. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    push_op_estack(setcolorspace_cont);

    while (code == 0 && depth) {
        ref_assign(&arr, ep);
        /* Run along the nested color spaces until we get to the lowest one
         * that we haven't yet processed (given by 'depth')
         */
        for (i = 0;i < depth;i++) {
            code = get_space_object(i_ctx_p, parr, &obj);
            if (code < 0)
                return code;

            if (i < (depth - 1)) {
                if (!obj->alternateproc) {
                    return_error(gs_error_typecheck);
                }
                code = obj->alternateproc(i_ctx_p, parr, &parr, &CIESubst);
                if (code < 0)
                    return code;
            }
        }

        code = obj->setproc(i_ctx_p, parr, &stage, &cont, CIESubst);
        make_int(pstage, stage);
        if (code != 0) {
            if (code < 0 && code != gs_error_stackoverflow)
                esp -= 5;
            return code;
        }
        if (!cont) {
            /* Completed that space, decrement the 'depth' */
            make_int(pdepth, --depth);
            parr = &arr;
        }
    }
    if (code == 0) {
        /* Remove our next continuation and our data */
        esp -= 5;
        op = osp;
        istate->colorspace[0].array = *op;
        /* Remove the colorspace array form the operand stack */
        pop(1);
        code = o_pop_estack;
    }
    return code;
}

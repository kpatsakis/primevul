currentbasecolor_cont(i_ctx_t *i_ctx_p)
{
    ref arr, *parr = &arr;
    es_ptr ep = esp;
    int i, code = 0, stage, base, cont=1, stack_depth = 0, CIESubst=0;
    unsigned int depth;
    PS_colour_space_t *obj;

    stack_depth = (int)ep[-4].value.intval;
    base = (int)ep[-3].value.intval;
    depth = (unsigned int)ep[-2].value.intval;
    stage = (int)ep[-1].value.intval;

    /* This shouldn't be possible, all the procedures which call this should
     * set the depth to at *least* 1.
     */
    if (depth < 1)
        return_error(gs_error_unknownerror);

    /* If we get a continuation from a sub-procedure, we will want to come back
     * here afterward, to do any remaining stages. We need to set up for that now.
     * so that our continuation is ahead of the sub-proc's continuation.
     */
    check_estack(1);
    /* The push_op_estack macro increments esp before use, so we don't need to */
    push_op_estack(currentbasecolor_cont);

    while (code == 0 && cont) {
        ref_assign(&arr, ep);
        parr = &arr;
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

        code = obj->basecolorproc(i_ctx_p, parr, base, &stage, &cont, &stack_depth);
        make_int(&ep[-4], stack_depth);
        make_int(&ep[-1], stage);
        if (code > 0)
            return code;
        /* Completed that space, increment the 'depth' */
        make_int(&ep[-2], ++depth);
    }
    if (code <= 0) {
        /* Remove our next continuation and our data */
        esp -= 7;
        code = o_pop_estack;
    }
    return code;
}

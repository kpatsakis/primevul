setcolorspace_nosubst(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    es_ptr ep;
    int code, depth;

    /* Make sure we have an operand... */
    check_op(1);
    /* Check its either a name (base space) or an array */
    if (!r_has_type(op, t_name))
        if (!r_is_array(op))
            return_error(gs_error_typecheck);

    code = validate_spaces(i_ctx_p, op, &depth);
    if (code < 0)
        return code;

    /* Set up for the continuation procedure which will do the work */
    /* Make sure the exec stack has enough space */
    check_estack(5);
    /* Store the initial value of CIE substitution (substituting) */
    ep = esp += 1;
    make_int(ep, 1);
    /* Store the 'depth' of the space returned during checking above */
    ep = esp += 1;
    make_int(ep, depth);
    /* Store the 'stage' of processing (initially 0) */
    ep = esp += 1;
    make_int(ep, 0);
    /* Store a pointer to the color space stored on the operand stack
     * as the stack may grow unpredictably making further access
     * to the space difficult
     */
    ep = esp += 1;
    *ep = *op;
    /* Finally, the actual continuation routine */
    push_op_estack(setcolorspace_cont);
    return o_push_estack;
}

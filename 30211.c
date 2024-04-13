static int ciebasecolor(i_ctx_t * i_ctx_p, ref *space, int base, int *stage, int *cont, int *stack_depth)
{
    os_ptr op;
    ref *spacename, nref;
    int i, components=1, code;

    /* If the spaece is an array, the first element is always the name */
    if (r_is_array(space))
        spacename = space->value.refs;
    else
        spacename = space;
    /* Check that it really is a name */
    if (!r_has_type(spacename, t_name))
        return_error(gs_error_typecheck);

    /* Find the relevant color space object */
    for (i=0;i<4;i++) {
        code = names_ref(imemory->gs_lib_ctx->gs_name_table, (const byte *)CIESpaces[i], strlen(CIESpaces[i]), &nref, 0);
        if (code < 0)
            return code;
        if (name_eq(spacename, &nref)) {
            break;
        }
    }
    /* Find out how many values are on the stack, which depends
     * on what kind of CIEBased space this is.
     */
    switch(i){
        case 0:
            components = 1;
            break;
        case 1:
        case 2:
            components = 3;
            break;
        case 3:
            components = 4;
            break;
    }
    /* Remove teh requisite number of values */
    pop(components);
    op = osp;
    /* Find out how many values we need to return, which
     * depends on the requested space.
     */
    switch(base) {
        case 0:
            components = 1;
            break;
        case 1:
        case 2:
            components = 3;
            break;
        case 3:
            components = 4;
            break;
    }
    push(components);
    /* The PLRM says that all the components should be returned as 0.0 */
    op -= components-1;
    for (i=0;i<components;i++) {
        make_real(op, (float)0);
        op++;
    }
    /* However, Adobe implementations actually return 1.0 for the black
     * channel of CMYK...
     */
    if (components == 4) {
        op--;
        make_real(op, (float)1);
    }
    *stage = 0;
    *cont = 0;
    return 0;
}

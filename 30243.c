int get_space_object(i_ctx_t *i_ctx_p, ref *arr, PS_colour_space_t **obj)
{
    ref spacename, nref;
    int i, nprocs = sizeof(colorProcs) / sizeof(PS_colour_space_t), code;

    /* If the spaece is an array, the first element is always the name */
    if (r_is_array(arr)) {
        code = array_get(imemory, arr, 0, &spacename);
        if (code < 0)
            return code;
    }
    else
        ref_assign(&spacename, arr);

    /* Check that it really is a name */
    if (!r_has_type(&spacename, t_name))
        return_error(gs_error_typecheck);

    /* Find the relevant color space object */
    for (i=0;i<nprocs;i++) {
        code = names_ref(imemory->gs_lib_ctx->gs_name_table, (const byte *)colorProcs[i].name, strlen(colorProcs[i].name), &nref, 0);
        if (code < 0)
            return code;
        if (name_eq(&spacename, &nref)) {
            *obj = &colorProcs[i];
            return 0;
        }
    }
    return_error(gs_error_undefined);
}

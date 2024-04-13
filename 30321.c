static int validateindexedspace(i_ctx_t * i_ctx_p, ref **space)
{
    int code = 0;
    ref *r = *space;
    ref nameref, sref, hival, lookup, altspace;

    if (!r_is_array(r))
        return_error(gs_error_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(r) != 4)
        return_error(gs_error_rangecheck);
    /* Check operand type(s) */
    /* Make sure 'hival' is an integer */
    code = array_get(imemory, r, 2, &hival);
    if (code < 0)
        return code;
    if (!r_has_type(&hival, t_integer))
        return_error(gs_error_typecheck);
    /* Make sure 'hival' lies between 0 and 4096 */
    if (hival.value.intval < 0 || hival.value.intval > 4096)
        return_error(gs_error_rangecheck);
    /* Ensure the 'lookup' is either a string or a procedure */
    code = array_get(imemory, r, 3, &lookup);
    if (code < 0)
        return code;
    if (!r_has_type(&lookup, t_string))
        check_proc(lookup);

    /* Get the name of the alternate space */
    code = array_get(imemory, r, 1, &altspace);
    if (code < 0)
        return code;
    if (r_has_type(&altspace, t_name))
        ref_assign(&nameref, &altspace);
    else {
        if (!r_is_array(&altspace))
            return_error(gs_error_typecheck);
        code = array_get(imemory, &altspace, 0, &nameref);
        if (code < 0)
            return code;
    }
    /* Convert alternate space name to string */
    name_string_ref(imemory, &nameref, &sref);
    /* Check its not /Indexed or /Pattern */
    if (r_size(&sref) == 7) {
        if (strncmp((const char *)sref.value.const_bytes, "Indexed", 7) == 0)
            return_error(gs_error_typecheck);
        if (strncmp((const char *)sref.value.const_bytes, "Pattern", 7) == 0)
            return_error(gs_error_typecheck);
    }
    ref_assign(*space, &altspace);
    return 0;
}

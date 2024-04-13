static int validatedevicenspace(i_ctx_t * i_ctx_p, ref **space)
{
    int i, code = 0;
    ref *devicenspace = *space, proc;
    ref nameref, sref, altspace, namesarray, sname;

    /* Check enough arguments in the space */
    if (r_size(devicenspace) < 4)
        return_error(gs_error_rangecheck);
    /* Check the names parameter is an array */
    code = array_get(imemory, devicenspace, 1, &namesarray);
    if (code < 0)
        return code;
    if (!r_is_array(&namesarray))
        return_error(gs_error_typecheck);
    /* Ensure we have at least one ink */
    if (r_size(&namesarray) < 1)
        return_error(gs_error_typecheck);
    /* Make sure no more inks than we can cope with */
    if (r_size(&namesarray) > MAX_COMPONENTS_IN_DEVN)    /* MUST match psi/icremap.h int_remap_color_info_s */
        return_error(gs_error_limitcheck);
    /* Check the tint transform is a procedure */
    code = array_get(imemory, devicenspace, 3, &proc);
    if (code < 0)
        return code;
    check_proc(proc);

    /* Check the array of ink names only contains names or strings */
    for (i = 0; i < r_size(&namesarray); ++i) {
        array_get(imemory, &namesarray, (long)i, &sname);
        switch (r_type(&sname)) {
                case t_string:
                case t_name:
                    break;
                default:
                    return_error(gs_error_typecheck);
        }
    }

    /* Get the name of the alternate space */
    code = array_get(imemory, devicenspace, 2, &altspace);
    if (code < 0)
        return code;
    if (r_has_type(&altspace, t_name))
        ref_assign(&nameref, &altspace);
    else {
        /* Make sure the alternate space is an array */
        if (!r_is_array(&altspace))
            return_error(gs_error_typecheck);
        /* And has a name for its type */
        code = array_get(imemory, &altspace, 0, &nameref);
        if (code < 0)
            return code;
        if (!r_has_type(&nameref, t_name))
            return_error(gs_error_typecheck);
    }
    /* Convert alternate space name to string */
    name_string_ref(imemory, &nameref, &sref);
    /* Check its not /Indexed, /Pattern, /DeviceN */
    if (r_size(&sref) == 7) {
        if (strncmp((const char *)sref.value.const_bytes, "Indexed", 7) == 0)
            return_error(gs_error_typecheck);
        if (strncmp((const char *)sref.value.const_bytes, "Pattern", 7) == 0)
            return_error(gs_error_typecheck);
        if (strncmp((const char *)sref.value.const_bytes, "DeviceN", 7) == 0)
            return_error(gs_error_typecheck);
    }
    /* and also not /Separation */
    if (r_size(&sref) == 9 && strncmp((const char *)sref.value.const_bytes, "Separation", 9) == 0)
           return_error(gs_error_typecheck);

    ref_assign(*space, &altspace);
    return 0;
}

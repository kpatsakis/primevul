static int validateciedefspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i;
    float value[6];
    ref     CIEdict, *pref, *CIEspace = *r, tempref, valref;

    if (!r_is_array(CIEspace))
        return_error(gs_error_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(CIEspace) != 2)
        return_error(gs_error_rangecheck);

    code = array_get(imemory, CIEspace, 1, &CIEdict);
    if (code < 0)
        return code;
    check_read_type(CIEdict, t_dictionary);

    code = validatecieabcspace(i_ctx_p, r);
    if (code != 0)
        return code;

    pref = &tempref;
    code = dict_find_string(&CIEdict, "Table", &pref);
    if (code > 0) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 4)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, pref, 3, value);
        if (code < 0)
            return code;
        if (value[0] <= 1 || value[1] <= 1 || value[2] <= 1)
            return_error(gs_error_rangecheck);

        code = array_get(imemory, pref, 3, &valref);
        if (code < 0)
            return code;
        if (!r_is_array(&valref))
            return_error(gs_error_typecheck);
        if (r_size(&valref) != value[0])
            return_error(gs_error_rangecheck);

        for (i=0;i<value[0];i++) {
            code = array_get(imemory, &valref, i, &tempref);
            if (code < 0)
                return code;
            if (!r_has_type(&tempref, t_string))
                return_error(gs_error_typecheck);

            if (r_size(&tempref) != (3 * value[1] * value[2]))
                return_error(gs_error_rangecheck);
        }
    } else {
        return_error(gs_error_rangecheck);
    }

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeDEF", &pref);
    if (code > 0 && !r_has_type(&tempref, t_null)) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 6)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, pref, 6, value);
        if (code < 0)
            return code;
        if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
            return_error(gs_error_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeDEF", &pref);
    if (code > 0 && !r_has_type(pref, t_null)) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 3)
            return_error(gs_error_rangecheck);

        for (i=0;i<3;i++) {
            code = array_get(imemory, pref, i, &valref);
            if (code < 0)
                return code;
            check_proc(valref);
        }
    }

    code = dict_find_string(&CIEdict, "RangeHIJ", &pref);
    if (code > 0 && !r_has_type(pref, t_null)) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 6)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, pref, 6, value);
        if (code < 0)
            return code;
        if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
            return_error(gs_error_rangecheck);
    }

    *r = 0;
    return 0;
}

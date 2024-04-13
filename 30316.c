static int validateciedefgspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i, j;
    float value[8];
    ref     CIEdict, *CIEspace = *r, tempref, arrayref, valref, *pref = &tempref;

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

    code = dict_find_string(&CIEdict, "Table", &pref);
    if (code > 0) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 5)
            return_error(gs_error_rangecheck);

        for (i=0;i<4;i++) {
            code = array_get(imemory, pref, i, &valref);
            if (code < 0)
                return code;
            if (r_has_type(&valref, t_integer))
                value[i] = (float)valref.value.intval;
            else
                return_error(gs_error_typecheck);
        }
        if (value[0] <= 1 || value[1] <= 1 || value[2] <= 1 || value[3] <= 1)
            return_error(gs_error_rangecheck);

        code = array_get(imemory, pref, 4, &arrayref);
        if (code < 0)
            return code;
        if (!r_is_array(&arrayref))
            return_error(gs_error_typecheck);
        if (r_size(&arrayref) != value[0])
            return_error(gs_error_rangecheck);

        for (i=0;i<value[0];i++) {
            code = array_get(imemory, &arrayref, i, &tempref);
            if (code < 0)
                return code;
            for (j=0;j<value[1];j++) {
                code = array_get(imemory, &tempref, i, &valref);
                if (code < 0)
                    return code;
                if (!r_has_type(&valref, t_string))
                    return_error(gs_error_typecheck);

                if (r_size(&valref) != (3 * value[2] * value[3]))
                    return_error(gs_error_rangecheck);
            }
        }
    } else {
        return_error(gs_error_rangecheck);
    }

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeDEFG", &pref);
    if (code > 0 && !r_has_type(pref, t_null)) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 8)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, pref, 8, value);
        if (code < 0)
            return code;
        if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4] || value[7] < value[6])
            return_error(gs_error_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeDEFG", &pref);
    if (code > 0 && !r_has_type(pref, t_null)) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 4)
            return_error(gs_error_rangecheck);

        for (i=0;i<4;i++) {
            code = array_get(imemory, pref, i, &valref);
            if (code < 0)
                return code;
            check_proc(valref);
        }
    }

    code = dict_find_string(&CIEdict, "RangeHIJK", &pref);
    if (code > 0 && !r_has_type(pref, t_null)) {
        if (!r_is_array(pref))
            return_error(gs_error_typecheck);
        if (r_size(pref) != 8)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, pref, 8, value);
        if (code < 0)
            return code;
        if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4] || value[7] < value[6])
            return_error(gs_error_rangecheck);
    }

    *r = 0;
    return 0;
}

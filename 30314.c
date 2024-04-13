static int validatecieabcspace(i_ctx_t * i_ctx_p, ref **r)
{
    int code = 0, i;
    float value[9];
    ref     CIEdict, *CIEspace = *r, *tempref, valref;

    if (!r_is_array(CIEspace))
        return_error(gs_error_typecheck);
    /* Validate parameters, check we have enough operands */
    if (r_size(CIEspace) != 2)
        return_error(gs_error_rangecheck);

    code = array_get(imemory, CIEspace, 1, &CIEdict);
    if (code < 0)
        return code;
    check_read_type(CIEdict, t_dictionary);

    /* Check white point exists, and is an array of three numbers */
    code = checkWhitePoint(i_ctx_p, &CIEdict);
    if (code != 0)
        return code;

    /* Remaining parameters are optional, but we must validate
     * them if they are present
     */
    code = dict_find_string(&CIEdict, "RangeABC", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (!r_is_array(tempref))
            return_error(gs_error_typecheck);
        if (r_size(tempref) != 6)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, tempref, 6, value);
        if (code < 0)
            return code;
        if (value[1] < value[0] || value[3] < value[2] || value[5] < value[4])
            return_error(gs_error_rangecheck);
    }

    code = dict_find_string(&CIEdict, "DecodeABC", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (!r_is_array(tempref))
            return_error(gs_error_typecheck);
        if (r_size(tempref) != 3)
            return_error(gs_error_rangecheck);

        for (i=0;i<3;i++) {
            code = array_get(imemory, tempref, i, &valref);
            if (code < 0)
                return code;
            check_proc(valref);
        }
    }

    code = dict_find_string(&CIEdict, "MatrixABC", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (!r_is_array(tempref))
            return_error(gs_error_typecheck);
        if (r_size(tempref) != 9)
            return_error(gs_error_rangecheck);
        code = get_cie_param_array(imemory, tempref, 9, value);
        if (code < 0)
            return code;
    }

    code = checkRangeLMN(i_ctx_p, &CIEdict);
    if (code != 0)
        return code;

    code = checkDecodeLMN(i_ctx_p, &CIEdict);
    if (code != 0)
        return code;

    code = checkMatrixLMN(i_ctx_p, &CIEdict);
    if (code != 0)
        return code;

    code = checkBlackPoint(i_ctx_p, &CIEdict);
    if (code != 0)
        return code;

    *r = 0;
    return 0;
}

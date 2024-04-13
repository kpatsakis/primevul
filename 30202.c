static int checkRangeLMN(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code;
    float value[6];
    ref *tempref;

    code = dict_find_string(CIEdict, "RangeLMN", &tempref);
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
    return 0;
}

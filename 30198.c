static int checkBlackPoint(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code;
    float value[3];
    ref *tempref;

    code = dict_find_string(CIEdict, "BlackPoint", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (!r_is_array(tempref))
            return_error(gs_error_typecheck);
        if (r_size(tempref) != 3)
            return_error(gs_error_rangecheck);

        code = get_cie_param_array(imemory, tempref, 3, value);
        if (code < 0)
            return code;
    }
    return 0;
}

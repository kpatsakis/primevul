static int checkWhitePoint(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code;
    float value[3];
    ref *tempref;

    code = dict_find_string(CIEdict, "WhitePoint", &tempref);
    if (code < 0)
        return code;

    if (code == 0 || r_has_type(tempref, t_null))
        return gs_note_error(gs_error_undefined);

    if (!r_is_array(tempref))
        return_error(gs_error_typecheck);
    if (r_size(tempref) != 3)
        return_error(gs_error_rangecheck);

    code = get_cie_param_array(imemory, tempref, 3, value);
    if (code < 0)
        return code;

    /* Xw and Zw must be positive and Yw must be 1 (3rd edition PLRM p230) */
    if (value[0] < 0 || value[1] != 1 || value[2] < 0 )
            return_error(gs_error_rangecheck);

    return 0;
}

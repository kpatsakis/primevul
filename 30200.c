static int checkDecodeLMN(i_ctx_t * i_ctx_p, ref *CIEdict)
{
    int code = 0, i;
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "DecodeLMN", &tempref);
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
    return 0;
}

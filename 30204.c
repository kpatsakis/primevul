static int checkrangeab(i_ctx_t * i_ctx_p, ref *labdict)
{
    int code = 0, i;
    float value[4];
    ref *tempref, valref;

    code = dict_find_string(labdict, "Range", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (!r_is_array(tempref))
            return_error(gs_error_typecheck);
        if (r_size(tempref) != 4)
            return_error(gs_error_rangecheck);

        for (i=0;i<4;i++) {
            code = array_get(imemory, tempref, i, &valref);
            if (code < 0)
                return code;
            if (r_has_type(&valref, t_integer))
                value[i] = (float)valref.value.intval;
            else if (r_has_type(&valref, t_real))
                value[i] = (float)valref.value.realval;
            else
                return_error(gs_error_typecheck);
        }
        if (value[1] < value[0] || value[3] < value[2] )
            return_error(gs_error_rangecheck);
    }
    return 0;
}

static int labdomain(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int i, code;
    ref     CIEdict, *tempref, valref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
        return code;

    /* If we have a Range, get the values from that */
    code = dict_find_string(&CIEdict, "Range", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        for (i=0;i<4;i++) {
            code = array_get(imemory, tempref, i, &valref);
            if (code < 0)
                return code;
            if (r_has_type(&valref, t_integer))
                ptr[i] = (float)valref.value.intval;
            else if (r_has_type(&valref, t_real))
                ptr[i] = (float)valref.value.realval;
            else
                return_error(gs_error_typecheck);
        }
    } else {
        /* Default values for Lab */
        for (i=0;i<2;i++) {
            ptr[2 * i] = -100;
            ptr[(2 * i) + 1] = 100;
        }
    }
    return 0;
}

static int ciedefgrange(i_ctx_t * i_ctx_p, ref *space, float *ptr)
{
    int code;
    ref CIEdict, *tempref;

    code = array_get(imemory, space, 1, &CIEdict);
    if (code < 0)
        return code;

    /* If we have a RangeDEFG, get the values from that */
    code = dict_find_string(&CIEdict, "RangeDEFG", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        code = get_cie_param_array(imemory, tempref, 8, ptr);
        if (code < 0)
            return code;
    } else {
        /* Default values for a CIEBasedDEFG */
    	memcpy(ptr, default_0_1, 8*sizeof(float));
    }
    return 0;
}

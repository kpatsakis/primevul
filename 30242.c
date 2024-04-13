static int get_cie_param_array(const gs_memory_t *mem, const ref *src,  int n, float *dst) {
	ref valref;
    int i;
    int code = 0;

    for (i = 0; i < n; i++) {
		code = array_get(mem, src, i, &valref);
		if (code < 0)
			return code;
		if (r_has_type(&valref, t_integer))
			dst[i] = (float)valref.value.intval;
		else if (r_has_type(&valref, t_real))
			dst[i] = (float)valref.value.realval;
		else
			return_error(gs_error_typecheck);
		if (dst[i] < -MAX_CIE_RANGE || dst[i] > MAX_CIE_RANGE)
			return_error(gs_error_limitcheck);
	}
    return code;
}

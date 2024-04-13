static void ebt_standard_compat_from_user(void *dst, const void *src)
{
	int v = *(compat_int_t *)src;

	if (v >= 0)
		v += xt_compat_calc_jump(NFPROTO_BRIDGE, v);
	memcpy(dst, &v, sizeof(v));
}

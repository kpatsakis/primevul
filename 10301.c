static char *__read_nonnull_str_at(RBuffer *buf, ut64 offset) {
	ut8 sz = r_buf_read8_at (buf, offset);
	if (!sz) {
		return NULL;
	}
	char *str = malloc ((ut64)sz + 1);
	if (!str) {
		return NULL;
	}
	r_buf_read_at (buf, offset + 1, (ut8 *)str, sz);
	str[sz] = '\0';
	return str;
}
static void utf8encode(zval *zv)
{
	size_t pos, len = 0;
	unsigned char *ptr = (unsigned char *) Z_STRVAL_P(zv);

	while (*ptr) {
		if (*ptr++ >= 0x80) {
			++len;
		}
		++len;
	}

	ptr = safe_emalloc(1, len, 1);
	for (len = 0, pos = 0; len <= Z_STRLEN_P(zv); ++len, ++pos) {
		ptr[pos] = Z_STRVAL_P(zv)[len];
		if ((ptr[pos]) >= 0x80) {
			ptr[pos + 1] = 0x80 | (ptr[pos] & 0x3f);
			ptr[pos] = 0xc0 | ((ptr[pos] >> 6) & 0x1f);
			++pos;
		}
	}
	zval_dtor(zv);
	ZVAL_STRINGL(zv, (char *) ptr, pos-1, 0);
}

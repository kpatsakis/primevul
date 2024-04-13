lou_translateString(const char *tableList, const widechar *inbufx, int *inlen,
		widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int mode) {
	return lou_translate(tableList, inbufx, inlen, outbuf, outlen, typeform, spacing,
			NULL, NULL, NULL, mode);
}

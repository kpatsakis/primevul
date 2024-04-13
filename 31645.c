static int pat_parse_meth(const char *text, struct pattern *pattern, int mflags, char **err)
{
	int len, meth;

	len  = strlen(text);
	meth = find_http_meth(text, len);

	pattern->val.i = meth;
	if (meth == HTTP_METH_OTHER) {
		pattern->ptr.str = (char *)text;
		pattern->len = len;
	}
	else {
		pattern->ptr.str = NULL;
		pattern->len = 0;
	}
	return 1;
}

static int http_replace_value(struct my_regex *re, char *dst, uint dst_size, char *val, int len, char delim,
                              const char *rep_str)
{
	char* p = val;
	char* dst_end = dst + dst_size;
	char* dst_p = dst;

	for (;;) {
		char *p_delim;

		/* look for delim. */
		p_delim = p;
		while (p_delim < p + len && *p_delim != delim)
			p_delim++;

		if (regex_exec_match2(re, p, p_delim-p, MAX_MATCH, pmatch)) {
			int replace_n = exp_replace(dst_p, dst_end - dst_p, p, rep_str, pmatch);

			if (replace_n < 0)
				return -1;

			dst_p += replace_n;
		} else {
			uint len = p_delim - p;

			if (dst_p + len >= dst_end)
				return -1;

			memcpy(dst_p, p, len);
			dst_p += len;
		}

		if (dst_p >= dst_end)
			return -1;

		/* end of the replacements. */
		if (p_delim >= p + len)
			break;

		/* Next part. */
		*dst_p++ = delim;
		p = p_delim + 1;
	}

	return dst_p - dst;
}

static int http_replace_header(struct my_regex *re, char *dst, uint dst_size, char *val, int len,
                               const char *rep_str)
{
	if (!regex_exec_match2(re, val, len, MAX_MATCH, pmatch))
		return -2;

	return exp_replace(dst, dst_size, val, rep_str, pmatch);
}

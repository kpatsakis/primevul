ftrace_match_record(struct dyn_ftrace *rec, char *mod,
		    char *regex, int len, int type)
{
	char str[KSYM_SYMBOL_LEN];
	char *modname;

	kallsyms_lookup(rec->ip, NULL, NULL, &modname, str);

	if (mod) {
		/* module lookup requires matching the module */
		if (!modname || strcmp(modname, mod))
			return 0;

		/* blank search means to match all funcs in the mod */
		if (!len)
			return 1;
	}

	return ftrace_match(str, regex, len, type);
}

ftrace_match_module_records(struct ftrace_hash *hash, char *buff, char *mod)
{
	int not = 0;

	/* blank or '*' mean the same */
	if (strcmp(buff, "*") == 0)
		buff[0] = 0;

	/* handle the case of 'dont filter this module' */
	if (strcmp(buff, "!") == 0 || strcmp(buff, "!*") == 0) {
		buff[0] = 0;
		not = 1;
	}

	return match_records(hash, buff, strlen(buff), mod, not);
}

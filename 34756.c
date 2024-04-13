static char *parse_and_add_kcmdline_module_options(char *options, const char *modulename)
{
	char *kcmdline_buf;
	char *kcmdline;
	char *kptr;
	int len;

	kcmdline_buf = xmalloc_open_read_close("/proc/cmdline", NULL);
	if (!kcmdline_buf)
		return options;

	kcmdline = kcmdline_buf;
	len = strlen(modulename);
	while ((kptr = strsep(&kcmdline, "\n\t ")) != NULL) {
		if (strncmp(modulename, kptr, len) != 0)
			continue;
		kptr += len;
		if (*kptr != '.')
			continue;
		/* It is "modulename.xxxx" */
		kptr++;
		if (strchr(kptr, '=') != NULL) {
			/* It is "modulename.opt=[val]" */
			options = gather_options_str(options, kptr);
		}
	}
	free(kcmdline_buf);

	return options;
}

int add_argv(const char *what, int quoted)
{
	DEBUGP("add_argv: %s\n", what);
	if (what && newargc + 1 < ARRAY_SIZE(newargv)) {
		newargv[newargc] = strdup(what);
		newargvattr[newargc] = quoted;
		newargv[++newargc] = NULL;
		return 1;
	} else {
		xtables_error(PARAMETER_PROBLEM,
			      "Parser cannot handle more arguments\n");
	}
}

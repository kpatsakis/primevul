ftrace_mod_callback(struct ftrace_hash *hash,
		    char *func, char *cmd, char *param, int enable)
{
	char *mod;
	int ret = -EINVAL;

	/*
	 * cmd == 'mod' because we only registered this func
	 * for the 'mod' ftrace_func_command.
	 * But if you register one func with multiple commands,
	 * you can tell which command was used by the cmd
	 * parameter.
	 */

	/* we must have a module name */
	if (!param)
		return ret;

	mod = strsep(&param, ":");
	if (!strlen(mod))
		return ret;

	ret = ftrace_match_module_records(hash, func, mod);
	if (!ret)
		ret = -EINVAL;
	if (ret < 0)
		return ret;

	return 0;
}

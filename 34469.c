read_etc_default_login(char ***env, u_int *envsize, uid_t uid)
{
	char **tmpenv = NULL, *var;
	u_int i, tmpenvsize = 0;
	u_long mask;

	/*
	 * We don't want to copy the whole file to the child's environment,
	 * so we use a temporary environment and copy the variables we're
	 * interested in.
	 */
	read_environment_file(&tmpenv, &tmpenvsize, "/etc/default/login");

	if (tmpenv == NULL)
		return;

	if (uid == 0)
		var = child_get_env(tmpenv, "SUPATH");
	else
		var = child_get_env(tmpenv, "PATH");
	if (var != NULL)
		child_set_env(env, envsize, "PATH", var);

	if ((var = child_get_env(tmpenv, "UMASK")) != NULL)
		if (sscanf(var, "%5lo", &mask) == 1)
			umask((mode_t)mask);

	for (i = 0; tmpenv[i] != NULL; i++)
		free(tmpenv[i]);
	free(tmpenv);
}

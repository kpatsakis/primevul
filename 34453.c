copy_environment(char **source, char ***env, u_int *envsize)
{
	char *var_name, *var_val;
	int i;

	if (source == NULL)
		return;

	for(i = 0; source[i] != NULL; i++) {
		var_name = xstrdup(source[i]);
		if ((var_val = strstr(var_name, "=")) == NULL) {
			free(var_name);
			continue;
		}
		*var_val++ = '\0';

		debug3("Copy environment: %s=%s", var_name, var_val);
		child_set_env(env, envsize, var_name, var_val);

		free(var_name);
	}
}

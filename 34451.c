child_get_env(char **env, const char *name)
{
	int i;
	size_t len;

	len = strlen(name);
	for (i=0; env[i] != NULL; i++)
		if (strncmp(name, env[i], len) == 0 && env[i][len] == '=')
			return(env[i] + len + 1);
	return NULL;
}

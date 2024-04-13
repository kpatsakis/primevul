static int read_config(const char *path)
{
	return recursive_action(path, ACTION_RECURSE | ACTION_QUIET,
				config_file_action, NULL, NULL, 1);
}

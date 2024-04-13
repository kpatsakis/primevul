static void argv_cleanup(struct subprocess_info *info)
{
	argv_free(info->argv);
}

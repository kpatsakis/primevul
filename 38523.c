void lbs_debugfs_init(void)
{
	if (!lbs_dir)
		lbs_dir = debugfs_create_dir("lbs_wireless", NULL);
}

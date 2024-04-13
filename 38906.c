ip_vs_use_count_inc(void)
{
	return try_module_get(THIS_MODULE);
}

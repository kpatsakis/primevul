need_secrets (NMSetting *setting)
{
	/* Assume that VPN connections need secrets since they almost always will */
	return g_ptr_array_sized_new (1);
}

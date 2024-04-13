nm_setting_vpn_remove_secret (NMSettingVPN *setting, const char *key)
{
	g_return_if_fail (NM_IS_SETTING_VPN (setting));

	g_hash_table_remove (NM_SETTING_VPN_GET_PRIVATE (setting)->secrets, key);
}

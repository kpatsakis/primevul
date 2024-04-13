nm_setting_vpn_add_secret (NMSettingVPN *setting,
                           const char *key,
                           const char *secret)
{
	g_return_if_fail (NM_IS_SETTING_VPN (setting));
	g_return_if_fail (key != NULL);
	g_return_if_fail (strlen (key) > 0);
	g_return_if_fail (secret != NULL);
	g_return_if_fail (strlen (secret) > 0);

	g_hash_table_insert (NM_SETTING_VPN_GET_PRIVATE (setting)->secrets,
	                     g_strdup (key), g_strdup (secret));
}

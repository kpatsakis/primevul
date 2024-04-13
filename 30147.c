nm_setting_vpn_foreach_secret (NMSettingVPN *setting,
                               NMVPNIterFunc func,
                               gpointer user_data)
{
	g_return_if_fail (setting != NULL);
	g_return_if_fail (NM_IS_SETTING_VPN (setting));

	foreach_item_helper (NM_SETTING_VPN_GET_PRIVATE (setting)->secrets, func, user_data);
}

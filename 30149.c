nm_setting_vpn_get_service_type (NMSettingVPN *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_VPN (setting), NULL);

	return NM_SETTING_VPN_GET_PRIVATE (setting)->service_type;
}

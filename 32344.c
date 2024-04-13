int script_config_tun(struct openconnect_info *vpninfo, const char *reason)
{
	if (!vpninfo->vpnc_script)
		return 0;

	setenv("reason", reason, 1);
	if (system(vpninfo->vpnc_script)) {
		int e = errno;
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to spawn script '%s' for %s: %s\n"),
			     vpninfo->vpnc_script, reason, strerror(e));
		return -e;
	}
	return 0;
}

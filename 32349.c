void shutdown_tun(struct openconnect_info *vpninfo)
{	
	if (vpninfo->script_tun) {
		kill(vpninfo->script_tun, SIGHUP);
	} else {
		script_config_tun(vpninfo, "disconnect");
#ifdef __sun__
		close(vpninfo->ip_fd);
		vpninfo->ip_fd = -1;
		if (vpninfo->ip6_fd != -1) {
			close(vpninfo->ip6_fd);
			vpninfo->ip6_fd = -1;
		}
#endif
	}

	close(vpninfo->tun_fd);
	vpninfo->tun_fd = -1;
}

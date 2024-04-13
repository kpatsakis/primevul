int setup_tun(struct openconnect_info *vpninfo)
{
	int tun_fd;

	set_script_env(vpninfo);

	if (vpninfo->script_tun) {
		pid_t child;
		int fds[2];

		if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fds)) {
			perror(_("socketpair"));
			exit(1);
		}
		tun_fd = fds[0];
		child = fork();
		if (child < 0) {
			perror(_("fork"));
			exit(1);
		} else if (!child) {
			close(tun_fd);
			setenv_int("VPNFD", fds[1]);
			execl("/bin/sh", "/bin/sh", "-c", vpninfo->vpnc_script, NULL);
			perror(_("execl"));
			exit(1);
		}
		close(fds[1]);
		vpninfo->script_tun = child;
		vpninfo->ifname = strdup(_("(script)"));
	} else {
		script_config_tun(vpninfo, "pre-init");

		tun_fd = os_setup_tun(vpninfo);
		if (tun_fd < 0)
			return tun_fd;

		setenv("TUNDEV", vpninfo->ifname, 1);
		script_config_tun(vpninfo, "connect");

		/* Ancient vpnc-scripts might not get this right */
		set_tun_mtu(vpninfo);
	}

	fcntl(tun_fd, F_SETFD, FD_CLOEXEC);

	vpninfo->tun_fd = tun_fd;

	if (vpninfo->select_nfds <= tun_fd)
		vpninfo->select_nfds = tun_fd + 1;

	FD_SET(tun_fd, &vpninfo->select_rfds);

	fcntl(vpninfo->tun_fd, F_SETFL, fcntl(vpninfo->tun_fd, F_GETFL) | O_NONBLOCK);

	return 0;
}

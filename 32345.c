static int set_tun_mtu(struct openconnect_info *vpninfo)
{
#ifndef __sun__ /* We don't know how to do this on Solaris */
	struct ifreq ifr;
	int net_fd;

	net_fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (net_fd < 0) {
		perror(_("open net"));
		return -EINVAL;
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, vpninfo->ifname, sizeof(ifr.ifr_name) - 1);
	ifr.ifr_mtu = vpninfo->mtu;

	if (ioctl(net_fd, SIOCSIFMTU, &ifr) < 0)
		perror(_("SIOCSIFMTU"));

	close(net_fd);
#endif
	return 0;
}

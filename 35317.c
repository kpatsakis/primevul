static int dev_ifname32(struct net *net, struct compat_ifreq __user *uifr32)
{
	struct ifreq __user *uifr;
	int err;

	uifr = compat_alloc_user_space(sizeof(struct ifreq));
	if (copy_in_user(uifr, uifr32, sizeof(struct compat_ifreq)))
		return -EFAULT;

	err = dev_ioctl(net, SIOCGIFNAME, uifr);
	if (err)
		return err;

	if (copy_in_user(uifr32, uifr, sizeof(struct compat_ifreq)))
		return -EFAULT;

	return 0;
}

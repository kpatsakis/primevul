static int compat_do_ebt_set_ctl(struct sock *sk,
		int cmd, void __user *user, unsigned int len)
{
	int ret;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case EBT_SO_SET_ENTRIES:
		ret = compat_do_replace(sock_net(sk), user, len);
		break;
	case EBT_SO_SET_COUNTERS:
		ret = compat_update_counters(sock_net(sk), user, len);
		break;
	default:
		ret = -EINVAL;
  }
	return ret;
}

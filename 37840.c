static int bnep_sock_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	if (cmd == BNEPGETCONNLIST) {
		struct bnep_connlist_req cl;
		uint32_t uci;
		int err;

		if (get_user(cl.cnum, (uint32_t __user *) arg) ||
				get_user(uci, (u32 __user *) (arg + 4)))
			return -EFAULT;

		cl.ci = compat_ptr(uci);

		if (cl.cnum <= 0)
			return -EINVAL;

		err = bnep_get_connlist(&cl);

		if (!err && put_user(cl.cnum, (uint32_t __user *) arg))
			err = -EFAULT;

		return err;
	}

	return bnep_sock_ioctl(sock, cmd, arg);
}

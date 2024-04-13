static long vsock_dev_do_ioctl(struct file *filp,
			       unsigned int cmd, void __user *ptr)
{
	u32 __user *p = ptr;
	int retval = 0;

	switch (cmd) {
	case IOCTL_VM_SOCKETS_GET_LOCAL_CID:
		if (put_user(transport->get_local_cid(), p) != 0)
			retval = -EFAULT;
		break;

	default:
		pr_err("Unknown ioctl %d\n", cmd);
		retval = -EINVAL;
	}

	return retval;
}

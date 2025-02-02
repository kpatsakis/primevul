static long aac_compat_do_ioctl(struct aac_dev *dev, unsigned cmd, unsigned long arg)
{
	long ret;
	mutex_lock(&aac_mutex);
	switch (cmd) {
	case FSACTL_MINIPORT_REV_CHECK:
	case FSACTL_SENDFIB:
	case FSACTL_OPEN_GET_ADAPTER_FIB:
	case FSACTL_CLOSE_GET_ADAPTER_FIB:
	case FSACTL_SEND_RAW_SRB:
	case FSACTL_GET_PCI_INFO:
	case FSACTL_QUERY_DISK:
	case FSACTL_DELETE_DISK:
	case FSACTL_FORCE_DELETE_DISK:
	case FSACTL_GET_CONTAINERS:
	case FSACTL_SEND_LARGE_FIB:
		ret = aac_do_ioctl(dev, cmd, (void __user *)arg);
		break;

	case FSACTL_GET_NEXT_ADAPTER_FIB: {
		struct fib_ioctl __user *f;

		f = compat_alloc_user_space(sizeof(*f));
		ret = 0;
		if (clear_user(f, sizeof(*f)))
			ret = -EFAULT;
		if (copy_in_user(f, (void __user *)arg, sizeof(struct fib_ioctl) - sizeof(u32)))
			ret = -EFAULT;
		if (!ret)
			ret = aac_do_ioctl(dev, cmd, f);
		break;
	}

	default:
		ret = -ENOIOCTLCMD;
		break;
	}
	mutex_unlock(&aac_mutex);
	return ret;
}

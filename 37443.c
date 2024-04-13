static long pmcraid_ioctl_driver(
	struct pmcraid_instance *pinstance,
	unsigned int cmd,
	unsigned int buflen,
	void __user *user_buffer
)
{
	int rc = -ENOSYS;

	if (!access_ok(VERIFY_READ, user_buffer, _IOC_SIZE(cmd))) {
		pmcraid_err("ioctl_driver: access fault in request buffer\n");
		return -EFAULT;
	}

	switch (cmd) {
	case PMCRAID_IOCTL_RESET_ADAPTER:
		pmcraid_reset_bringup(pinstance);
		rc = 0;
		break;

	default:
		break;
	}

	return rc;
}

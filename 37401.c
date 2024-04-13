static int pmcraid_check_ioctl_buffer(
	int cmd,
	void __user *arg,
	struct pmcraid_ioctl_header *hdr
)
{
	int rc = 0;
	int access = VERIFY_READ;

	if (copy_from_user(hdr, arg, sizeof(struct pmcraid_ioctl_header))) {
		pmcraid_err("couldn't copy ioctl header from user buffer\n");
		return -EFAULT;
	}

	/* check for valid driver signature */
	rc = memcmp(hdr->signature,
		    PMCRAID_IOCTL_SIGNATURE,
		    sizeof(hdr->signature));
	if (rc) {
		pmcraid_err("signature verification failed\n");
		return -EINVAL;
	}

	/* check for appropriate buffer access */
	if ((_IOC_DIR(cmd) & _IOC_READ) == _IOC_READ)
		access = VERIFY_WRITE;

	rc = access_ok(access,
		       (arg + sizeof(struct pmcraid_ioctl_header)),
		       hdr->buffer_length);
	if (!rc) {
		pmcraid_err("access failed for user buffer of size %d\n",
			     hdr->buffer_length);
		return -EFAULT;
	}

	return 0;
}

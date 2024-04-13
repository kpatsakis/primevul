static long pmcraid_chr_ioctl(
	struct file *filep,
	unsigned int cmd,
	unsigned long arg
)
{
	struct pmcraid_instance *pinstance = NULL;
	struct pmcraid_ioctl_header *hdr = NULL;
	int retval = -ENOTTY;

	hdr = kmalloc(GFP_KERNEL, sizeof(struct pmcraid_ioctl_header));

	if (!hdr) {
		pmcraid_err("faile to allocate memory for ioctl header\n");
		return -ENOMEM;
	}

	retval = pmcraid_check_ioctl_buffer(cmd, (void *)arg, hdr);

	if (retval) {
		pmcraid_info("chr_ioctl: header check failed\n");
		kfree(hdr);
		return retval;
	}

	pinstance = filep->private_data;

	if (!pinstance) {
		pmcraid_info("adapter instance is not found\n");
		kfree(hdr);
		return -ENOTTY;
	}

	switch (_IOC_TYPE(cmd)) {

	case PMCRAID_PASSTHROUGH_IOCTL:
		/* If ioctl code is to download microcode, we need to block
		 * mid-layer requests.
		 */
		if (cmd == PMCRAID_IOCTL_DOWNLOAD_MICROCODE)
			scsi_block_requests(pinstance->host);

		retval = pmcraid_ioctl_passthrough(pinstance,
						   cmd,
						   hdr->buffer_length,
						   arg);

		if (cmd == PMCRAID_IOCTL_DOWNLOAD_MICROCODE)
			scsi_unblock_requests(pinstance->host);
		break;

	case PMCRAID_DRIVER_IOCTL:
		arg += sizeof(struct pmcraid_ioctl_header);
		retval = pmcraid_ioctl_driver(pinstance,
					      cmd,
					      hdr->buffer_length,
					      (void __user *)arg);
		break;

	default:
		retval = -ENOTTY;
		break;
	}

	kfree(hdr);

	return retval;
}

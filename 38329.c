static int aac_ioctl(struct scsi_device *sdev, int cmd, void __user * arg)
{
	struct aac_dev *dev = (struct aac_dev *)sdev->host->hostdata;
	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	return aac_do_ioctl(dev, cmd, arg);
}

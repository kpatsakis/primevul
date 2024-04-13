static ssize_t lbs_lowsnr_write(struct file *file, const char __user *userbuf,
				size_t count, loff_t *ppos)
{
	return lbs_threshold_write(TLV_TYPE_SNR_LOW, CMD_SUBSCRIBE_SNR_LOW,
				   file, userbuf, count, ppos);
}

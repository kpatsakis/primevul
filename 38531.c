static ssize_t lbs_highsnr_write(struct file *file, const char __user *userbuf,
				 size_t count, loff_t *ppos)
{
	return lbs_threshold_write(TLV_TYPE_SNR_HIGH, CMD_SUBSCRIBE_SNR_HIGH,
				   file, userbuf, count, ppos);
}

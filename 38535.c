static ssize_t lbs_lowrssi_write(struct file *file, const char __user *userbuf,
				 size_t count, loff_t *ppos)
{
	return lbs_threshold_write(TLV_TYPE_RSSI_LOW, CMD_SUBSCRIBE_RSSI_LOW,
				   file, userbuf, count, ppos);
}

static ssize_t lbs_highrssi_write(struct file *file, const char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	return lbs_threshold_write(TLV_TYPE_RSSI_HIGH, CMD_SUBSCRIBE_RSSI_HIGH,
				   file, userbuf, count, ppos);
}

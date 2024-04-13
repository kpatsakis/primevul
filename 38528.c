static ssize_t lbs_failcount_write(struct file *file, const char __user *userbuf,
				   size_t count, loff_t *ppos)
{
	return lbs_threshold_write(TLV_TYPE_FAILCOUNT, CMD_SUBSCRIBE_FAILCOUNT,
				   file, userbuf, count, ppos);
}

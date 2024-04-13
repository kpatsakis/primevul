static ssize_t lbs_bcnmiss_read(struct file *file, char __user *userbuf,
				size_t count, loff_t *ppos)
{
	return lbs_threshold_read(TLV_TYPE_BCNMISS, CMD_SUBSCRIBE_BCNMISS,
				  file, userbuf, count, ppos);
}

static ssize_t lbs_sleepparams_read(struct file *file, char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t ret;
	size_t pos = 0;
	struct sleep_params sp;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	ret = lbs_cmd_802_11_sleep_params(priv, CMD_ACT_GET, &sp);
	if (ret)
		goto out_unlock;

	pos += snprintf(buf, len, "%d %d %d %d %d %d\n", sp.sp_error,
			sp.sp_offset, sp.sp_stabletime,
			sp.sp_calcontrol, sp.sp_extsleepclk,
			sp.sp_reserved);

	ret = simple_read_from_buffer(userbuf, count, ppos, buf, pos);

out_unlock:
	free_page(addr);
	return ret;
}

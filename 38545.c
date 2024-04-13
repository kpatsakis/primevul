static ssize_t lbs_sleepparams_write(struct file *file,
				const char __user *user_buf, size_t count,
				loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t buf_size, ret;
	struct sleep_params sp;
	int p1, p2, p3, p4, p5, p6;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	buf_size = min(count, len - 1);
	if (copy_from_user(buf, user_buf, buf_size)) {
		ret = -EFAULT;
		goto out_unlock;
	}
	ret = sscanf(buf, "%d %d %d %d %d %d", &p1, &p2, &p3, &p4, &p5, &p6);
	if (ret != 6) {
		ret = -EINVAL;
		goto out_unlock;
	}
	sp.sp_error = p1;
	sp.sp_offset = p2;
	sp.sp_stabletime = p3;
	sp.sp_calcontrol = p4;
	sp.sp_extsleepclk = p5;
	sp.sp_reserved = p6;

	ret = lbs_cmd_802_11_sleep_params(priv, CMD_ACT_SET, &sp);
	if (!ret)
		ret = count;
	else if (ret > 0)
		ret = -EINVAL;

out_unlock:
	free_page(addr);
	return ret;
}

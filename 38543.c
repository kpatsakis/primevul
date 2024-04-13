static ssize_t lbs_rdrf_write(struct file *file,
				    const char __user *userbuf,
				    size_t count, loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t res, buf_size;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	buf_size = min(count, len - 1);
	if (copy_from_user(buf, userbuf, buf_size)) {
		res = -EFAULT;
		goto out_unlock;
	}
	priv->rf_offset = simple_strtoul(buf, NULL, 16);
	res = count;
out_unlock:
	free_page(addr);
	return res;
}

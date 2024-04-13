static ssize_t lbs_host_sleep_read(struct file *file, char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t ret;
	size_t pos = 0;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	pos += snprintf(buf, len, "%d\n", priv->is_host_sleep_activated);

	ret = simple_read_from_buffer(userbuf, count, ppos, buf, pos);

	free_page(addr);
	return ret;
}

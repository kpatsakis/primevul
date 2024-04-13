static ssize_t lbs_dev_info(struct file *file, char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	size_t pos = 0;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	ssize_t res;
	if (!buf)
		return -ENOMEM;

	pos += snprintf(buf+pos, len-pos, "state = %s\n",
				szStates[priv->connect_status]);
	pos += snprintf(buf+pos, len-pos, "region_code = %02x\n",
				(u32) priv->regioncode);

	res = simple_read_from_buffer(userbuf, count, ppos, buf, pos);

	free_page(addr);
	return res;
}

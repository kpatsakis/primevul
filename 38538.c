static ssize_t lbs_rdbbp_read(struct file *file, char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t pos = 0;
	int ret;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	u32 val;

	if (!buf)
		return -ENOMEM;

	ret = lbs_get_reg(priv, CMD_BBP_REG_ACCESS, priv->bbp_offset, &val);
	mdelay(10);
	if (!ret) {
		pos = snprintf(buf, len, "BBP[0x%x] = 0x%08x\n",
				priv->bbp_offset, val);
		ret = simple_read_from_buffer(userbuf, count, ppos, buf, pos);
	}
	free_page(addr);

	return ret;
}

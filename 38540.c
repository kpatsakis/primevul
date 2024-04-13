static ssize_t lbs_rdmac_read(struct file *file, char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t pos = 0;
	int ret;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	u32 val = 0;

	if (!buf)
		return -ENOMEM;

	ret = lbs_get_reg(priv, CMD_MAC_REG_ACCESS, priv->mac_offset, &val);
	mdelay(10);
	if (!ret) {
		pos = snprintf(buf, len, "MAC[0x%x] = 0x%08x\n",
				priv->mac_offset, val);
		ret = simple_read_from_buffer(userbuf, count, ppos, buf, pos);
	}
	free_page(addr);
	return ret;
}

static ssize_t lbs_wrmac_write(struct file *file,
				    const char __user *userbuf,
				    size_t count, loff_t *ppos)
{

	struct lbs_private *priv = file->private_data;
	ssize_t res, buf_size;
	u32 offset, value;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	buf_size = min(count, len - 1);
	if (copy_from_user(buf, userbuf, buf_size)) {
		res = -EFAULT;
		goto out_unlock;
	}
	res = sscanf(buf, "%x %x", &offset, &value);
	if (res != 2) {
		res = -EFAULT;
		goto out_unlock;
	}

	res = lbs_set_reg(priv, CMD_MAC_REG_ACCESS, offset, value);
	mdelay(10);

	if (!res)
		res = count;
out_unlock:
	free_page(addr);
	return res;
}

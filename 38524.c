static ssize_t lbs_debugfs_read(struct file *file, char __user *userbuf,
			size_t count, loff_t *ppos)
{
	int val = 0;
	size_t pos = 0;
	ssize_t res;
	char *p;
	int i;
	struct debug_data *d;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	p = buf;

	d = file->private_data;

	for (i = 0; i < num_of_items; i++) {
		if (d[i].size == 1)
			val = *((u8 *) d[i].addr);
		else if (d[i].size == 2)
			val = *((u16 *) d[i].addr);
		else if (d[i].size == 4)
			val = *((u32 *) d[i].addr);
		else if (d[i].size == 8)
			val = *((u64 *) d[i].addr);

		pos += sprintf(p + pos, "%s=%d\n", d[i].name, val);
	}

	res = simple_read_from_buffer(userbuf, count, ppos, p, pos);

	free_page(addr);
	return res;
}

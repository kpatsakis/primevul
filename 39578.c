ftrace_pid_write(struct file *filp, const char __user *ubuf,
		   size_t cnt, loff_t *ppos)
{
	char buf[64], *tmp;
	long val;
	int ret;

	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(&buf, ubuf, cnt))
		return -EFAULT;

	buf[cnt] = 0;

	/*
	 * Allow "echo > set_ftrace_pid" or "echo -n '' > set_ftrace_pid"
	 * to clean the filter quietly.
	 */
	tmp = strstrip(buf);
	if (strlen(tmp) == 0)
		return 1;

	ret = kstrtol(tmp, 10, &val);
	if (ret < 0)
		return ret;

	ret = ftrace_pid_add(val);

	return ret ? ret : cnt;
}

static ssize_t lbs_host_sleep_write(struct file *file,
				const char __user *user_buf, size_t count,
				loff_t *ppos)
{
	struct lbs_private *priv = file->private_data;
	ssize_t buf_size, ret;
	int host_sleep;
	unsigned long addr = get_zeroed_page(GFP_KERNEL);
	char *buf = (char *)addr;
	if (!buf)
		return -ENOMEM;

	buf_size = min(count, len - 1);
	if (copy_from_user(buf, user_buf, buf_size)) {
		ret = -EFAULT;
		goto out_unlock;
	}
	ret = sscanf(buf, "%d", &host_sleep);
	if (ret != 1) {
		ret = -EINVAL;
		goto out_unlock;
	}

	if (host_sleep == 0)
		ret = lbs_set_host_sleep(priv, 0);
	else if (host_sleep == 1) {
		if (priv->wol_criteria == EHS_REMOVE_WAKEUP) {
			netdev_info(priv->dev,
				    "wake parameters not configured\n");
			ret = -EINVAL;
			goto out_unlock;
		}
		ret = lbs_set_host_sleep(priv, 1);
	} else {
		netdev_err(priv->dev, "invalid option\n");
		ret = -EINVAL;
	}

	if (!ret)
		ret = count;

out_unlock:
	free_page(addr);
	return ret;
}

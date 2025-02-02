static ssize_t oz_cdev_read(struct file *filp, char __user *buf, size_t count,
		loff_t *fpos)
{
	int n;
	int ix;

	struct oz_pd *pd;
	struct oz_serial_ctx *ctx;

	spin_lock_bh(&g_cdev.lock);
	pd = g_cdev.active_pd;
	if (pd)
		oz_pd_get(pd);
	spin_unlock_bh(&g_cdev.lock);
	if (pd == NULL)
		return -1;
	ctx = oz_cdev_claim_ctx(pd);
	if (ctx == NULL)
		goto out2;
	n = ctx->rd_in - ctx->rd_out;
	if (n < 0)
		n += OZ_RD_BUF_SZ;
	if (count > n)
		count = n;
	ix = ctx->rd_out;
	n = OZ_RD_BUF_SZ - ix;
	if (n > count)
		n = count;
	if (copy_to_user(buf, &ctx->rd_buf[ix], n)) {
		count = 0;
		goto out1;
	}
	ix += n;
	if (ix == OZ_RD_BUF_SZ)
		ix = 0;
	if (n < count) {
		if (copy_to_user(&buf[n], ctx->rd_buf, count-n)) {
			count = 0;
			goto out1;
		}
		ix = count-n;
	}
	ctx->rd_out = ix;
out1:
	oz_cdev_release_ctx(ctx);
out2:
	oz_pd_put(pd);
	return count;
}

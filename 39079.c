static unsigned int oz_cdev_poll(struct file *filp, poll_table *wait)
{
	unsigned int ret = 0;
	struct oz_cdev *dev = filp->private_data;

	oz_dbg(ON, "Poll called wait = %p\n", wait);
	spin_lock_bh(&dev->lock);
	if (dev->active_pd) {
		struct oz_serial_ctx *ctx = oz_cdev_claim_ctx(dev->active_pd);
		if (ctx) {
			if (ctx->rd_in != ctx->rd_out)
				ret |= POLLIN | POLLRDNORM;
			oz_cdev_release_ctx(ctx);
		}
	}
	spin_unlock_bh(&dev->lock);
	if (wait)
		poll_wait(filp, &dev->rdq, wait);
	return ret;
}

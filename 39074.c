static struct oz_serial_ctx *oz_cdev_claim_ctx(struct oz_pd *pd)
{
	struct oz_serial_ctx *ctx;

	spin_lock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
	ctx = (struct oz_serial_ctx *)pd->app_ctx[OZ_APPID_SERIAL-1];
	if (ctx)
		atomic_inc(&ctx->ref_count);
	spin_unlock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
	return ctx;
}

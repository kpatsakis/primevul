int oz_cdev_start(struct oz_pd *pd, int resume)
{
	struct oz_serial_ctx *ctx;
	struct oz_serial_ctx *old_ctx;

	if (resume) {
		oz_dbg(ON, "Serial service resumed\n");
		return 0;
	}
	ctx = kzalloc(sizeof(struct oz_serial_ctx), GFP_ATOMIC);
	if (ctx == NULL)
		return -ENOMEM;
	atomic_set(&ctx->ref_count, 1);
	ctx->tx_seq_num = 1;
	spin_lock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
	old_ctx = pd->app_ctx[OZ_APPID_SERIAL-1];
	if (old_ctx) {
		spin_unlock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
		kfree(ctx);
	} else {
		pd->app_ctx[OZ_APPID_SERIAL-1] = ctx;
		spin_unlock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
	}
	spin_lock(&g_cdev.lock);
	if ((g_cdev.active_pd == NULL) &&
		(memcmp(pd->mac_addr, g_cdev.active_addr, ETH_ALEN) == 0)) {
		oz_pd_get(pd);
		g_cdev.active_pd = pd;
		oz_dbg(ON, "Active PD arrived\n");
	}
	spin_unlock(&g_cdev.lock);
	oz_dbg(ON, "Serial service started\n");
	return 0;
}

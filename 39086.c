void oz_cdev_stop(struct oz_pd *pd, int pause)
{
	struct oz_serial_ctx *ctx;

	if (pause) {
		oz_dbg(ON, "Serial service paused\n");
		return;
	}
	spin_lock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
	ctx = (struct oz_serial_ctx *)pd->app_ctx[OZ_APPID_SERIAL-1];
	pd->app_ctx[OZ_APPID_SERIAL-1] = NULL;
	spin_unlock_bh(&pd->app_lock[OZ_APPID_SERIAL-1]);
	if (ctx)
		oz_cdev_release_ctx(ctx);
	spin_lock(&g_cdev.lock);
	if (pd == g_cdev.active_pd)
		g_cdev.active_pd = NULL;
	else
		pd = NULL;
	spin_unlock(&g_cdev.lock);
	if (pd) {
		oz_pd_put(pd);
		oz_dbg(ON, "Active PD departed\n");
	}
	oz_dbg(ON, "Serial service stopped\n");
}

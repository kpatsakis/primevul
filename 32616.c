struct appctx *stream_int_register_handler(struct stream_interface *si, struct si_applet *app)
{
	struct appctx *appctx;

	DPRINTF(stderr, "registering handler %p for si %p (was %p)\n", app, si, si->owner);

	appctx = si_alloc_appctx(si);
	if (!si)
		return NULL;

	appctx_set_applet(appctx, app);
	si->flags |= SI_FL_WAIT_DATA;
	return si_appctx(si);
}

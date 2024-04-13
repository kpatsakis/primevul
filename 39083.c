static void oz_cdev_release_ctx(struct oz_serial_ctx *ctx)
{
	if (atomic_dec_and_test(&ctx->ref_count)) {
		oz_dbg(ON, "Dealloc serial context\n");
		kfree(ctx);
	}
}

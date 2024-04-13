static void user_ta_ctx_destroy(struct tee_ta_ctx *ctx)
{
	free_utc(to_user_ta_ctx(ctx));
}

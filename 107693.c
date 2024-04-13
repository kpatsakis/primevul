static void set_ta_ctx_ops(struct tee_ta_ctx *ctx)
{
	ctx->ops = _user_ta_ops;
}

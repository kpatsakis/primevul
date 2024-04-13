bool is_user_ta_ctx(struct tee_ta_ctx *ctx)
{
	return ctx->ops == _user_ta_ops;
}

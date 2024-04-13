static int vrend_decode_create_sub_ctx(struct vrend_decode_ctx *ctx, int length)
{
   if (length != 1)
      return EINVAL;

   uint32_t ctx_sub_id = get_buf_entry(ctx, 1);

   vrend_renderer_create_sub_ctx(ctx->grctx, ctx_sub_id);
   return 0;
}

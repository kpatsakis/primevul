static int vrend_decode_end_query(struct vrend_decode_ctx *ctx, int length)
{
   if (length != 1)
      return EINVAL;

   uint32_t handle = get_buf_entry(ctx, VIRGL_QUERY_END_HANDLE);

   vrend_end_query(ctx->grctx, handle);
   return 0;
}

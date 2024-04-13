static int vrend_decode_get_query_result(struct vrend_decode_ctx *ctx, int length)
{
   if (length != 2)
      return EINVAL;

   uint32_t handle = get_buf_entry(ctx, VIRGL_QUERY_RESULT_HANDLE);
   uint32_t wait = get_buf_entry(ctx, VIRGL_QUERY_RESULT_WAIT);

   vrend_get_query_result(ctx->grctx, handle, wait);
   return 0;
}

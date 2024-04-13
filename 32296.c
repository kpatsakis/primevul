static int vrend_decode_set_index_buffer(struct vrend_decode_ctx *ctx, int length)
{
   if (length != 1 && length != 3)
      return EINVAL;
   vrend_set_index_buffer(ctx->grctx,
                          get_buf_entry(ctx, VIRGL_SET_INDEX_BUFFER_HANDLE),
                          (length == 3) ? get_buf_entry(ctx, VIRGL_SET_INDEX_BUFFER_INDEX_SIZE) : 0,
                          (length == 3) ? get_buf_entry(ctx, VIRGL_SET_INDEX_BUFFER_OFFSET) : 0);
   return 0;
}

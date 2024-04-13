static int vrend_decode_create_stream_output_target(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   uint32_t res_handle, buffer_size, buffer_offset;

   if (length != VIRGL_OBJ_STREAMOUT_SIZE)
      return EINVAL;

   res_handle = get_buf_entry(ctx, VIRGL_OBJ_STREAMOUT_RES_HANDLE);
   buffer_offset = get_buf_entry(ctx, VIRGL_OBJ_STREAMOUT_BUFFER_OFFSET);
   buffer_size = get_buf_entry(ctx, VIRGL_OBJ_STREAMOUT_BUFFER_SIZE);

   return vrend_create_so_target(ctx->grctx, handle, res_handle, buffer_offset,
                                 buffer_size);
}

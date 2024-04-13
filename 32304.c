static int vrend_decode_set_uniform_buffer(struct vrend_decode_ctx *ctx, int length)
{
   if (length != VIRGL_SET_UNIFORM_BUFFER_SIZE)
      return EINVAL;

   uint32_t shader = get_buf_entry(ctx, VIRGL_SET_UNIFORM_BUFFER_SHADER_TYPE);
   uint32_t index = get_buf_entry(ctx, VIRGL_SET_UNIFORM_BUFFER_INDEX);
   uint32_t offset = get_buf_entry(ctx, VIRGL_SET_UNIFORM_BUFFER_OFFSET);
   uint32_t blength = get_buf_entry(ctx, VIRGL_SET_UNIFORM_BUFFER_LENGTH);
   uint32_t handle = get_buf_entry(ctx, VIRGL_SET_UNIFORM_BUFFER_RES_HANDLE);

   if (shader >= PIPE_SHADER_TYPES)
      return EINVAL;

   if (index >= PIPE_MAX_CONSTANT_BUFFERS)
      return EINVAL;

   vrend_set_uniform_buffer(ctx->grctx, shader, index, offset, blength, handle);
   return 0;
}

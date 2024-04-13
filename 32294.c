static int vrend_decode_set_constant_buffer(struct vrend_decode_ctx *ctx, uint16_t length)
{
   uint32_t shader;
   uint32_t index;
   int nc = (length - 2);

   if (length < 2)
      return EINVAL;

   shader = get_buf_entry(ctx, VIRGL_SET_CONSTANT_BUFFER_SHADER_TYPE);
   index = get_buf_entry(ctx, VIRGL_SET_CONSTANT_BUFFER_INDEX);

   if (shader >= PIPE_SHADER_TYPES)
      return EINVAL;

   vrend_set_constants(ctx->grctx, shader, index, nc, get_buf_ptr(ctx, VIRGL_SET_CONSTANT_BUFFER_DATA_START));
   return 0;
}

static int vrend_decode_create_shader(struct vrend_decode_ctx *ctx,
                                      uint32_t handle,
                                      uint16_t length)
{
   struct pipe_stream_output_info so_info;
   int i, ret;
   uint32_t shader_offset;
   unsigned num_tokens, num_so_outputs, offlen;
   uint8_t *shd_text;
   uint32_t type;

   if (length < 5)
      return EINVAL;

   type = get_buf_entry(ctx, VIRGL_OBJ_SHADER_TYPE);
   num_tokens = get_buf_entry(ctx, VIRGL_OBJ_SHADER_NUM_TOKENS);
   offlen = get_buf_entry(ctx, VIRGL_OBJ_SHADER_OFFSET);
   num_so_outputs = get_buf_entry(ctx, VIRGL_OBJ_SHADER_SO_NUM_OUTPUTS);

   if (num_so_outputs > PIPE_MAX_SO_OUTPUTS)
      return EINVAL;

   shader_offset = 6;
   if (num_so_outputs) {
      so_info.num_outputs = num_so_outputs;
      if (so_info.num_outputs) {
         for (i = 0; i < 4; i++)
            so_info.stride[i] = get_buf_entry(ctx, VIRGL_OBJ_SHADER_SO_STRIDE(i));
         for (i = 0; i < so_info.num_outputs; i++) {
            uint32_t tmp = get_buf_entry(ctx, VIRGL_OBJ_SHADER_SO_OUTPUT0(i));

            so_info.output[i].register_index = tmp & 0xff;
            so_info.output[i].start_component = (tmp >> 8) & 0x3;
            so_info.output[i].num_components = (tmp >> 10) & 0x7;
            so_info.output[i].output_buffer = (tmp >> 13) & 0x7;
            so_info.output[i].dst_offset = (tmp >> 16) & 0xffff;
         }
      }
      shader_offset += 4 + (2 * num_so_outputs);
   } else
     memset(&so_info, 0, sizeof(so_info));

   shd_text = get_buf_ptr(ctx, shader_offset);
   ret = vrend_create_shader(ctx->grctx, handle, &so_info, (const char *)shd_text, offlen, num_tokens, type, length - shader_offset + 1);

   return ret;
}

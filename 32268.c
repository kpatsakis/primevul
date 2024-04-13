static int vrend_decode_bind_sampler_states(struct vrend_decode_ctx *ctx, int length)
{
   if (length < 2)
      return EINVAL;

   uint32_t shader_type = get_buf_entry(ctx, VIRGL_BIND_SAMPLER_STATES_SHADER_TYPE);
   uint32_t start_slot = get_buf_entry(ctx, VIRGL_BIND_SAMPLER_STATES_START_SLOT);
   uint32_t num_states = length - 2;

   if (shader_type >= PIPE_SHADER_TYPES)
      return EINVAL;

   vrend_bind_sampler_states(ctx->grctx, shader_type, start_slot, num_states,
                             get_buf_ptr(ctx, VIRGL_BIND_SAMPLER_STATES_S0_HANDLE));
   return 0;
}

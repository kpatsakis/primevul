static int vrend_decode_set_sampler_views(struct vrend_decode_ctx *ctx, uint16_t length)
{
   int num_samps;
   int i;
   uint32_t shader_type, start_slot;

   if (length < 2)
      return EINVAL;
   num_samps = length - 2;
   shader_type = get_buf_entry(ctx, VIRGL_SET_SAMPLER_VIEWS_SHADER_TYPE);
   start_slot = get_buf_entry(ctx, VIRGL_SET_SAMPLER_VIEWS_START_SLOT);

   if (shader_type >= PIPE_SHADER_TYPES)
      return EINVAL;

   if (num_samps > PIPE_MAX_SHADER_SAMPLER_VIEWS ||
       start_slot > (PIPE_MAX_SHADER_SAMPLER_VIEWS - num_samps))
      return EINVAL;

   for (i = 0; i < num_samps; i++) {
      uint32_t handle = get_buf_entry(ctx, VIRGL_SET_SAMPLER_VIEWS_V0_HANDLE + i);
      vrend_set_single_sampler_view(ctx->grctx, shader_type, i + start_slot, handle);
   }
   vrend_set_num_sampler_views(ctx->grctx, shader_type, start_slot, num_samps);
   return 0;
}

static int vrend_decode_create_sampler_state(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   struct pipe_sampler_state state;
   int i;
   uint32_t tmp;

   if (length != VIRGL_OBJ_SAMPLER_STATE_SIZE)
      return EINVAL;
   tmp = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_STATE_S0);
   state.wrap_s = tmp & 0x7;
   state.wrap_t = (tmp >> 3) & 0x7;
   state.wrap_r = (tmp >> 6) & 0x7;
   state.min_img_filter = (tmp >> 9) & 0x3;
   state.min_mip_filter = (tmp >> 11) & 0x3;
   state.mag_img_filter = (tmp >> 13) & 0x3;
   state.compare_mode = (tmp >> 15) & 0x1;
   state.compare_func = (tmp >> 16) & 0x7;

   state.lod_bias = uif(get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_STATE_LOD_BIAS));
   state.min_lod = uif(get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_STATE_MIN_LOD));
   state.max_lod = uif(get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_STATE_MAX_LOD));

   for (i = 0; i < 4; i++)
      state.border_color.ui[i] = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_STATE_BORDER_COLOR(i));
   return vrend_create_sampler_state(ctx->grctx, handle, &state);
}

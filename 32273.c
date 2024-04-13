static int vrend_decode_create_blend(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   struct pipe_blend_state *blend_state;
   uint32_t tmp;
   int i;

   if (length != VIRGL_OBJ_BLEND_SIZE) {
      return EINVAL;
   }

   blend_state = CALLOC_STRUCT(pipe_blend_state);
   if (!blend_state)
      return ENOMEM;

   tmp = get_buf_entry(ctx, VIRGL_OBJ_BLEND_S0);
   blend_state->independent_blend_enable = (tmp & 1);
   blend_state->logicop_enable = (tmp >> 1) & 0x1;
   blend_state->dither = (tmp >> 2) & 0x1;
   blend_state->alpha_to_coverage = (tmp >> 3) & 0x1;
   blend_state->alpha_to_one = (tmp >> 4) & 0x1;

   tmp = get_buf_entry(ctx, VIRGL_OBJ_BLEND_S1);
   blend_state->logicop_func = tmp & 0xf;

   for (i = 0; i < PIPE_MAX_COLOR_BUFS; i++) {
      tmp = get_buf_entry(ctx, VIRGL_OBJ_BLEND_S2(i));
      blend_state->rt[i].blend_enable = tmp & 0x1;
      blend_state->rt[i].rgb_func = (tmp >> 1) & 0x7;
      blend_state->rt[i].rgb_src_factor = (tmp >> 4) & 0x1f;
      blend_state->rt[i].rgb_dst_factor = (tmp >> 9) & 0x1f;
      blend_state->rt[i].alpha_func = (tmp >> 14) & 0x7;
      blend_state->rt[i].alpha_src_factor = (tmp >> 17) & 0x1f;
      blend_state->rt[i].alpha_dst_factor = (tmp >> 22) & 0x1f;
      blend_state->rt[i].colormask = (tmp >> 27) & 0xf;
   }

   tmp = vrend_renderer_object_insert(ctx->grctx, blend_state, sizeof(struct pipe_blend_state), handle,
                                      VIRGL_OBJECT_BLEND);
   if (tmp == 0) {
      FREE(blend_state);
      return ENOMEM;
   }
   return 0;
}

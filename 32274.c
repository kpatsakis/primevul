static int vrend_decode_create_dsa(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   int i;
   struct pipe_depth_stencil_alpha_state *dsa_state;
   uint32_t tmp;

   if (length != VIRGL_OBJ_DSA_SIZE)
      return EINVAL;

   dsa_state = CALLOC_STRUCT(pipe_depth_stencil_alpha_state);
   if (!dsa_state)
      return ENOMEM;

   tmp = get_buf_entry(ctx, VIRGL_OBJ_DSA_S0);
   dsa_state->depth.enabled = tmp & 0x1;
   dsa_state->depth.writemask = (tmp >> 1) & 0x1;
   dsa_state->depth.func = (tmp >> 2) & 0x7;

   dsa_state->alpha.enabled = (tmp >> 8) & 0x1;
   dsa_state->alpha.func = (tmp >> 9) & 0x7;

   for (i = 0; i < 2; i++) {
      tmp = get_buf_entry(ctx, VIRGL_OBJ_DSA_S1 + i);
      dsa_state->stencil[i].enabled = tmp & 0x1;
      dsa_state->stencil[i].func = (tmp >> 1) & 0x7;
      dsa_state->stencil[i].fail_op = (tmp >> 4) & 0x7;
      dsa_state->stencil[i].zpass_op = (tmp >> 7) & 0x7;
      dsa_state->stencil[i].zfail_op = (tmp >> 10) & 0x7;
      dsa_state->stencil[i].valuemask = (tmp >> 13) & 0xff;
      dsa_state->stencil[i].writemask = (tmp >> 21) & 0xff;
   }

   tmp = get_buf_entry(ctx, VIRGL_OBJ_DSA_ALPHA_REF);
   dsa_state->alpha.ref_value = uif(tmp);

   tmp = vrend_renderer_object_insert(ctx->grctx, dsa_state, sizeof(struct pipe_depth_stencil_alpha_state), handle,
                                      VIRGL_OBJECT_DSA);
   if (tmp == 0) {
      FREE(dsa_state);
      return ENOMEM;
   }
   return 0;
}

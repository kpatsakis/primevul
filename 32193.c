static void vrend_hw_emit_dsa(struct vrend_context *ctx)
{
   struct pipe_depth_stencil_alpha_state *state = &ctx->sub->dsa_state;

   if (state->depth.enabled) {
      vrend_depth_test_enable(ctx, true);
      glDepthFunc(GL_NEVER + state->depth.func);
      if (state->depth.writemask)
         glDepthMask(GL_TRUE);
      else
         glDepthMask(GL_FALSE);
   } else
      vrend_depth_test_enable(ctx, false);

   if (state->alpha.enabled) {
      vrend_alpha_test_enable(ctx, true);
      if (!vrend_state.use_core_profile)
         glAlphaFunc(GL_NEVER + state->alpha.func, state->alpha.ref_value);
   } else
      vrend_alpha_test_enable(ctx, false);


}

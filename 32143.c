static void vrend_alpha_test_enable(struct vrend_context *ctx, bool alpha_test_enable)
{
   if (vrend_state.use_core_profile) {
      /* handled in shaders */
      return;
   }
   if (ctx->sub->alpha_test_enabled != alpha_test_enable) {
      ctx->sub->alpha_test_enabled = alpha_test_enable;
      if (alpha_test_enable)
         glEnable(GL_ALPHA_TEST);
      else
         glDisable(GL_ALPHA_TEST);
   }
}

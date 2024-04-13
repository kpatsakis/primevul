static void vrend_stencil_test_enable(struct vrend_context *ctx, bool stencil_test_enable)
{
   if (ctx->sub->stencil_test_enabled != stencil_test_enable) {
      ctx->sub->stencil_test_enabled = stencil_test_enable;
      if (stencil_test_enable)
         glEnable(GL_STENCIL_TEST);
      else
         glDisable(GL_STENCIL_TEST);
   }
}

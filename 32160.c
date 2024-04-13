static void vrend_depth_test_enable(struct vrend_context *ctx, bool depth_test_enable)
{
   if (ctx->sub->depth_test_enabled != depth_test_enable) {
      ctx->sub->depth_test_enabled = depth_test_enable;
      if (depth_test_enable)
         glEnable(GL_DEPTH_TEST);
      else
         glDisable(GL_DEPTH_TEST);
   }
}

static void vrend_hw_emit_framebuffer_state(struct vrend_context *ctx)
{
   static const GLenum buffers[8] = {
      GL_COLOR_ATTACHMENT0_EXT,
      GL_COLOR_ATTACHMENT1_EXT,
      GL_COLOR_ATTACHMENT2_EXT,
      GL_COLOR_ATTACHMENT3_EXT,
      GL_COLOR_ATTACHMENT4_EXT,
      GL_COLOR_ATTACHMENT5_EXT,
      GL_COLOR_ATTACHMENT6_EXT,
      GL_COLOR_ATTACHMENT7_EXT,
   };
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ctx->sub->fb_id);

   if (ctx->sub->nr_cbufs == 0) {
      glReadBuffer(GL_NONE);
      glDisable(GL_FRAMEBUFFER_SRGB_EXT);
   } else {
      struct vrend_surface *surf = NULL;
      int i;
      for (i = 0; i < ctx->sub->nr_cbufs; i++) {
         if (ctx->sub->surf[i]) {
            surf = ctx->sub->surf[i];
         }
      }
      if (util_format_is_srgb(surf->format)) {
         glEnable(GL_FRAMEBUFFER_SRGB_EXT);
      } else {
         glDisable(GL_FRAMEBUFFER_SRGB_EXT);
      }
   }
   glDrawBuffers(ctx->sub->nr_cbufs, buffers);
}

static void vrend_hw_set_zsurf_texture(struct vrend_context *ctx)
{
   struct vrend_resource *tex;
   int first_layer, last_layer;
   if (!ctx->sub->zsurf) {
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_STENCIL_ATTACHMENT,
                                GL_TEXTURE_2D, 0, 0);
      return;

   }
   tex = ctx->sub->zsurf->texture;
   if (!tex)
      return;

   first_layer = ctx->sub->zsurf->val1 & 0xffff;
   last_layer = (ctx->sub->zsurf->val1 >> 16) & 0xffff;

   vrend_fb_bind_texture(tex, 0, ctx->sub->zsurf->val0,
                         first_layer != last_layer ? 0xffffffff : first_layer);
}

static void vrend_blend_enable(struct vrend_context *ctx, bool blend_enable)
{
   if (ctx->sub->blend_enabled != blend_enable) {
      ctx->sub->blend_enabled = blend_enable;
      if (blend_enable)
         glEnable(GL_BLEND);
      else
         glDisable(GL_BLEND);
   }
}

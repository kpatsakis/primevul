static bool blit_build_vs_passthrough(struct vrend_blitter_ctx *blit_ctx)
{
   blit_ctx->vs = glCreateShader(GL_VERTEX_SHADER);

   if (!build_and_check(blit_ctx->vs, VS_PASSTHROUGH)) {
      glDeleteShader(blit_ctx->vs);
      blit_ctx->vs = 0;
      return false;
   }
   return true;
}

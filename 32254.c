void vrend_set_polygon_stipple(struct vrend_context *ctx,
                               struct pipe_poly_stipple *ps)
{
   if (vrend_state.use_core_profile) {
      static const unsigned bit31 = 1 << 31;
      GLubyte *stip = calloc(1, 1024);
      int i, j;

      if (!ctx->pstip_inited)
         vrend_init_pstipple_texture(ctx);

      if (!stip)
         return;

      for (i = 0; i < 32; i++) {
         for (j = 0; j < 32; j++) {
            if (ps->stipple[i] & (bit31 >> j))
               stip[i * 32 + j] = 0;
            else
               stip[i * 32 + j] = 255;
         }
      }

      glBindTexture(GL_TEXTURE_2D, ctx->pstipple_tex_id);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 32, 32,
                      GL_RED, GL_UNSIGNED_BYTE, stip);

      free(stip);
      return;
   }
   glPolygonStipple((const GLubyte *)ps->stipple);
}

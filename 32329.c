static GLuint blit_get_frag_tex_col_emu_alpha(struct vrend_blitter_ctx *blit_ctx, int pipe_tex_target, unsigned nr_samples)
{
   assert(pipe_tex_target < PIPE_MAX_TEXTURE_TYPES);

   if (nr_samples > 1) {
      return 0;
   } else {
      GLuint *shader = &blit_ctx->fs_texfetch_col_emu_alpha[pipe_tex_target];

      if (!*shader) {
         unsigned tgsi_tex = util_pipe_tex_to_tgsi_tex(pipe_tex_target, 0);

         *shader = blit_build_frag_tex_col_emu_alpha(blit_ctx, tgsi_tex);
      }
      return *shader;
   }
}

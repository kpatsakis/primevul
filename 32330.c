static GLuint blit_get_frag_tex_writedepth(struct vrend_blitter_ctx *blit_ctx, int pipe_tex_target, unsigned nr_samples)
{
   assert(pipe_tex_target < PIPE_MAX_TEXTURE_TYPES);

   if (nr_samples > 1) {
      GLuint *shader = &blit_ctx->fs_texfetch_depth_msaa[pipe_tex_target];

      if (!*shader) {
         unsigned tgsi_tex = util_pipe_tex_to_tgsi_tex(pipe_tex_target, nr_samples);

         *shader = blit_build_frag_blit_msaa_depth(blit_ctx, tgsi_tex);
      }
      return *shader;

   } else {
      GLuint *shader = &blit_ctx->fs_texfetch_depth[pipe_tex_target];

      if (!*shader) {
         unsigned tgsi_tex = util_pipe_tex_to_tgsi_tex(pipe_tex_target, 0);

         *shader = blit_build_frag_tex_writedepth(blit_ctx, tgsi_tex);
      }
      return *shader;
   }
}

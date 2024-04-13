static GLuint blit_build_frag_blit_msaa_depth(struct vrend_blitter_ctx *blit_ctx, int tgsi_tex_target)
{
   GLuint fs_id;
   char shader_buf[4096];
   int is_shad;
   const char *twm;
   const char *ivec;

   switch (tgsi_tex_target) {
   case TGSI_TEXTURE_2D_MSAA:
      twm = ".xy";
      ivec = "ivec2";
      break;
   case TGSI_TEXTURE_2D_ARRAY_MSAA:
      twm = ".xyz";
      ivec = "ivec3";
      break;
   default:
      return 0;
   }

   snprintf(shader_buf, 4096, FS_TEXFETCH_DS_MSAA, vrend_shader_samplertypeconv(tgsi_tex_target, &is_shad), ivec, twm);

   fs_id = glCreateShader(GL_FRAGMENT_SHADER);

   if (!build_and_check(fs_id, shader_buf)) {
      glDeleteShader(fs_id);
      return 0;
   }

   return fs_id;
}

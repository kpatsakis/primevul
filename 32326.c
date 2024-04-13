static GLuint blit_build_frag_tex_col_emu_alpha(struct vrend_blitter_ctx *blit_ctx, int tgsi_tex_target)
{
   GLuint fs_id;
   char shader_buf[4096];
   int is_shad;
   const char *twm;
   const char *ext_str = "";
   switch (tgsi_tex_target) {
   case TGSI_TEXTURE_1D:
   case TGSI_TEXTURE_BUFFER:
      twm = ".x";
      break;
   case TGSI_TEXTURE_1D_ARRAY:
   case TGSI_TEXTURE_2D:
   case TGSI_TEXTURE_RECT:
   case TGSI_TEXTURE_2D_MSAA:
   default:
      twm = ".xy";
      break;
   case TGSI_TEXTURE_SHADOW1D:
   case TGSI_TEXTURE_SHADOW2D:
   case TGSI_TEXTURE_SHADOW1D_ARRAY:
   case TGSI_TEXTURE_SHADOWRECT:
   case TGSI_TEXTURE_3D:
   case TGSI_TEXTURE_CUBE:
   case TGSI_TEXTURE_2D_ARRAY:
   case TGSI_TEXTURE_2D_ARRAY_MSAA:
      twm = ".xyz";
      break;
   case TGSI_TEXTURE_SHADOWCUBE:
   case TGSI_TEXTURE_SHADOW2D_ARRAY:
   case TGSI_TEXTURE_SHADOWCUBE_ARRAY:
   case TGSI_TEXTURE_CUBE_ARRAY:
      twm = "";
      break;
   }

   if (tgsi_tex_target == TGSI_TEXTURE_CUBE_ARRAY ||
       tgsi_tex_target == TGSI_TEXTURE_SHADOWCUBE_ARRAY)
      ext_str = "#extension GL_ARB_texture_cube_map_array : require\n";

   snprintf(shader_buf, 4096, FS_TEXFETCH_COL_ALPHA_DEST, ext_str, vrend_shader_samplertypeconv(tgsi_tex_target, &is_shad), twm, "");

   fs_id = glCreateShader(GL_FRAGMENT_SHADER);

   if (!build_and_check(fs_id, shader_buf)) {
      glDeleteShader(fs_id);
      return 0;
   }

   return fs_id;
}

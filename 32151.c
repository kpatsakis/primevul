static bool vrend_compile_shader(struct vrend_context *ctx,
                                 struct vrend_shader *shader)
{
   GLint param;
   glShaderSource(shader->id, 1, (const char **)&shader->glsl_prog, NULL);
   glCompileShader(shader->id);
   glGetShaderiv(shader->id, GL_COMPILE_STATUS, &param);
   if (param == GL_FALSE) {
      char infolog[65536];
      int len;
      glGetShaderInfoLog(shader->id, 65536, &len, infolog);
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_SHADER, 0);
      fprintf(stderr,"shader failed to compile\n%s\n", infolog);
      fprintf(stderr,"GLSL:\n%s\n", shader->glsl_prog);
      return false;
   }
   return true;
}

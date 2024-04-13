static bool build_and_check(GLuint id, const char *buf)
{
   GLint param;
   glShaderSource(id, 1, (const char **)&buf, NULL);
   glCompileShader(id);

   glGetShaderiv(id, GL_COMPILE_STATUS, &param);
   if (param == GL_FALSE) {
      char infolog[65536];
      int len;
      glGetShaderInfoLog(id, 65536, &len, infolog);
      fprintf(stderr,"shader failed to compile\n%s\n", infolog);
      fprintf(stderr,"GLSL:\n%s\n", buf);
      return false;
   }
   return true;
}

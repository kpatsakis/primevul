static GLenum get_xfb_mode(GLenum mode)
{
   switch (mode) {
   case GL_POINTS:
      return GL_POINTS;
   case GL_TRIANGLES:
   case GL_TRIANGLE_STRIP:
   case GL_TRIANGLE_FAN:
   case GL_QUADS:
   case GL_QUAD_STRIP:
   case GL_POLYGON:
      return GL_TRIANGLES;
   case GL_LINES:
   case GL_LINE_LOOP:
   case GL_LINE_STRIP:
      return GL_LINES;
   default:
      fprintf(stderr, "failed to translate TFB %d\n", mode);
      return GL_POINTS;
   }
}

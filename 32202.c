static bool vrend_is_timer_query(GLenum gltype)
{
   return gltype == GL_TIMESTAMP ||
      gltype == GL_TIME_ELAPSED;
}

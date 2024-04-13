GLint64 vrend_renderer_get_timestamp(void)
{
   GLint64 v;
   glGetInteger64v(GL_TIMESTAMP, &v);
   return v;
}

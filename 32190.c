static bool vrend_get_one_query_result(GLuint query_id, bool use_64, uint64_t *result)
{
   GLint ready;
   GLuint passed;
   GLuint64 pass64;

   glGetQueryObjectiv(query_id, GL_QUERY_RESULT_AVAILABLE_ARB, &ready);

   if (!ready)
      return false;

   if (use_64) {
      glGetQueryObjectui64v(query_id, GL_QUERY_RESULT_ARB, &pass64);
      *result = pass64;
   } else {
      glGetQueryObjectuiv(query_id, GL_QUERY_RESULT_ARB, &passed);
      *result = passed;
   }
   return true;
}

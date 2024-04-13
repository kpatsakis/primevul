static boolean parse_identifier( const char **pcur, char *ret, size_t len )
{
   const char *cur = *pcur;
   int i = 0;
   if (is_alpha_underscore( cur )) {
      ret[i++] = *cur++;
      while (is_alpha_underscore( cur ) || is_digit( cur )) {
         if (i == len - 1)
            return FALSE;
         ret[i++] = *cur++;
      }
      ret[i++] = '\0';
      *pcur = cur;
      return TRUE;
   }
   return FALSE;
}

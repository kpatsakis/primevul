static boolean str_match_no_case( const char **pcur, const char *str )
{
   const char *cur = *pcur;

   while (*str != '\0' && *str == uprcase( *cur )) {
      str++;
      cur++;
   }
   if (*str == '\0') {
      *pcur = cur;
      return TRUE;
   }
   return FALSE;
}

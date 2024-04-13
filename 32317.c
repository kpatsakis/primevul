static boolean str_match_nocase_whole( const char **pcur, const char *str )
{
   const char *cur = *pcur;

   if (str_match_no_case(&cur, str) &&
       !is_digit_alpha_underscore(cur)) {
      *pcur = cur;
      return TRUE;
   }
   return FALSE;
}

static boolean parse_int( const char **pcur, int *val )
{
   const char *cur = *pcur;
   int sign = (*cur == '-' ? -1 : 1);

   if (*cur == '+' || *cur == '-')
      cur++;

   if (parse_uint(&cur, (uint *)val)) {
      *val *= sign;
      *pcur = cur;
      return TRUE;
   }

    return FALSE;
 }

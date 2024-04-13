streq_nocase_uprcase(const char *str1,
                     const char *str2)
{
   while (*str1 && *str2) {
      if (*str1 != uprcase(*str2))
         return FALSE;
      str1++;
      str2++;
   }
   return *str1 == 0 && *str2 == 0;
}

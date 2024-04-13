static boolean parse_double( const char **pcur, uint32_t *val0, uint32_t *val1)
{
   const char *cur = *pcur;
   union {
      double dval;
      uint32_t uval[2];
   } v;

   v.dval = strtod(cur, (char**)pcur);
   if (*pcur == cur)
      return FALSE;

   *val0 = v.uval[0];
   *val1 = v.uval[1];

   return TRUE;
}

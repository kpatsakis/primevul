parse_primitive( const char **pcur, uint *primitive )
{
   uint i;

   for (i = 0; i < PIPE_PRIM_MAX; i++) {
      const char *cur = *pcur;

      if (str_match_nocase_whole( &cur, tgsi_primitive_names[i])) {
         *primitive = i;
         *pcur = cur;
         return TRUE;
      }
   }
   return FALSE;
}

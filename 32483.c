parse_file( const char **pcur, uint *file )
{
   uint i;

   for (i = 0; i < TGSI_FILE_COUNT; i++) {
      const char *cur = *pcur;

      if (str_match_nocase_whole( &cur, tgsi_file_name(i) )) {
         *pcur = cur;
         *file = i;
         return TRUE;
      }
   }
   return FALSE;
}

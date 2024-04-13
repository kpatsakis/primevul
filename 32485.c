parse_fs_coord_origin( const char **pcur, uint *fs_coord_origin )
{
   uint i;

   for (i = 0; i < Elements(tgsi_fs_coord_origin_names); i++) {
      const char *cur = *pcur;

      if (str_match_nocase_whole( &cur, tgsi_fs_coord_origin_names[i])) {
         *fs_coord_origin = i;
         *pcur = cur;
         return TRUE;
      }
   }
   return FALSE;
}

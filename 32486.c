parse_fs_coord_pixel_center( const char **pcur, uint *fs_coord_pixel_center )
{
   uint i;

   for (i = 0; i < Elements(tgsi_fs_coord_pixel_center_names); i++) {
      const char *cur = *pcur;

      if (str_match_nocase_whole( &cur, tgsi_fs_coord_pixel_center_names[i])) {
         *fs_coord_pixel_center = i;
         *pcur = cur;
         return TRUE;
      }
   }
   return FALSE;
}

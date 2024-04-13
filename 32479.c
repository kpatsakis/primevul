match_inst(const char **pcur,
           unsigned *saturate,
           const struct tgsi_opcode_info *info)
{
   const char *cur = *pcur;

   /* simple case: the whole string matches the instruction name */
   if (str_match_nocase_whole(&cur, info->mnemonic)) {
      *pcur = cur;
      *saturate = 0;
      return TRUE;
   }

   if (str_match_no_case(&cur, info->mnemonic)) {
      /* the instruction has a suffix, figure it out */
      if (str_match_nocase_whole(&cur, "_SAT")) {
         *pcur = cur;
         *saturate = 1;
         return TRUE;
      }
   }

   return FALSE;
}

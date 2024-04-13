parse_register_dst(
   struct translate_ctx *ctx,
   uint *file,
   struct parsed_bracket *brackets)
{
   brackets->ind_comp = TGSI_SWIZZLE_X;
   if (!parse_register_file_bracket( ctx, file ))
      return FALSE;
   if (!parse_register_bracket( ctx, brackets ))
       return FALSE;

   return TRUE;
}

parse_dst_operand(
   struct translate_ctx *ctx,
   struct tgsi_full_dst_register *dst )
{
   uint file;
   uint writemask;
   const char *cur;
   struct parsed_bracket bracket[2];
   int parsed_opt_brackets;

   if (!parse_register_dst( ctx, &file, &bracket[0] ))
      return FALSE;
   if (!parse_opt_register_src_bracket(ctx, &bracket[1], &parsed_opt_brackets))
      return FALSE;

   cur = ctx->cur;
   eat_opt_white( &cur );

   if (!parse_opt_writemask( ctx, &writemask ))
      return FALSE;

   dst->Register.File = file;
   if (parsed_opt_brackets) {
      dst->Register.Dimension = 1;
      dst->Dimension.Indirect = 0;
      dst->Dimension.Dimension = 0;
      dst->Dimension.Index = bracket[0].index;

      if (bracket[0].ind_file != TGSI_FILE_NULL) {
         dst->Dimension.Indirect = 1;
         dst->DimIndirect.File = bracket[0].ind_file;
         dst->DimIndirect.Index = bracket[0].ind_index;
         dst->DimIndirect.Swizzle = bracket[0].ind_comp;
         dst->DimIndirect.ArrayID = bracket[0].ind_array;
      }
      bracket[0] = bracket[1];
   }
   dst->Register.Index = bracket[0].index;
   dst->Register.WriteMask = writemask;
   if (bracket[0].ind_file != TGSI_FILE_NULL) {
      dst->Register.Indirect = 1;
      dst->Indirect.File = bracket[0].ind_file;
      dst->Indirect.Index = bracket[0].ind_index;
      dst->Indirect.Swizzle = bracket[0].ind_comp;
      dst->Indirect.ArrayID = bracket[0].ind_array;
   }
   return TRUE;
}

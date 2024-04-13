parse_src_operand(
   struct translate_ctx *ctx,
   struct tgsi_full_src_register *src )
{
   uint file;
   uint swizzle[4];
   boolean parsed_swizzle;
   struct parsed_bracket bracket[2];
   int parsed_opt_brackets;

   if (*ctx->cur == '-') {
      ctx->cur++;
      eat_opt_white( &ctx->cur );
      src->Register.Negate = 1;
   }

   if (*ctx->cur == '|') {
      ctx->cur++;
      eat_opt_white( &ctx->cur );
      src->Register.Absolute = 1;
   }

   if (!parse_register_src(ctx, &file, &bracket[0]))
      return FALSE;
   if (!parse_opt_register_src_bracket(ctx, &bracket[1], &parsed_opt_brackets))
      return FALSE;

   src->Register.File = file;
   if (parsed_opt_brackets) {
      src->Register.Dimension = 1;
      src->Dimension.Indirect = 0;
      src->Dimension.Dimension = 0;
      src->Dimension.Index = bracket[0].index;
      if (bracket[0].ind_file != TGSI_FILE_NULL) {
         src->Dimension.Indirect = 1;
         src->DimIndirect.File = bracket[0].ind_file;
         src->DimIndirect.Index = bracket[0].ind_index;
         src->DimIndirect.Swizzle = bracket[0].ind_comp;
         src->DimIndirect.ArrayID = bracket[0].ind_array;
      }
      bracket[0] = bracket[1];
   }
   src->Register.Index = bracket[0].index;
   if (bracket[0].ind_file != TGSI_FILE_NULL) {
      src->Register.Indirect = 1;
      src->Indirect.File = bracket[0].ind_file;
      src->Indirect.Index = bracket[0].ind_index;
      src->Indirect.Swizzle = bracket[0].ind_comp;
      src->Indirect.ArrayID = bracket[0].ind_array;
   }

   /* Parse optional swizzle.
    */
   if (parse_optional_swizzle( ctx, swizzle, &parsed_swizzle, 4 )) {
      if (parsed_swizzle) {
         src->Register.SwizzleX = swizzle[0];
         src->Register.SwizzleY = swizzle[1];
         src->Register.SwizzleZ = swizzle[2];
         src->Register.SwizzleW = swizzle[3];
      }
   }

   if (src->Register.Absolute) {
      eat_opt_white( &ctx->cur );
      if (*ctx->cur != '|') {
         report_error( ctx, "Expected `|'" );
         return FALSE;
      }
      ctx->cur++;
   }


   return TRUE;
}

static boolean translate( struct translate_ctx *ctx )
{
   eat_opt_white( &ctx->cur );
   if (!parse_header( ctx ))
      return FALSE;

   if (ctx->processor == TGSI_PROCESSOR_TESS_CTRL ||
       ctx->processor == TGSI_PROCESSOR_TESS_EVAL)
       ctx->implied_array_size = 32;

   while (*ctx->cur != '\0') {
      uint label_val = 0;
      if (!eat_white( &ctx->cur )) {
         report_error( ctx, "Syntax error" );
         return FALSE;
      }

      if (*ctx->cur == '\0')
         break;
      if (parse_label( ctx, &label_val )) {
         if (!parse_instruction( ctx, TRUE ))
            return FALSE;
      }
      else if (str_match_nocase_whole( &ctx->cur, "DCL" )) {
         if (!parse_declaration( ctx ))
            return FALSE;
      }
      else if (str_match_nocase_whole( &ctx->cur, "IMM" )) {
         if (!parse_immediate( ctx ))
            return FALSE;
      }
      else if (str_match_nocase_whole( &ctx->cur, "PROPERTY" )) {
         if (!parse_property( ctx ))
            return FALSE;
      }
      else if (!parse_instruction( ctx, FALSE )) {
         return FALSE;
      }
   }

   return TRUE;
}

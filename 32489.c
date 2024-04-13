static boolean parse_immediate( struct translate_ctx *ctx )
{
   struct tgsi_full_immediate imm;
   uint advance;
   int type;

   if (*ctx->cur == '[') {
      uint uindex;

      ++ctx->cur;

      eat_opt_white( &ctx->cur );
      if (!parse_uint( &ctx->cur, &uindex )) {
         report_error( ctx, "Expected literal unsigned integer" );
         return FALSE;
      }

      if (uindex != ctx->num_immediates) {
         report_error( ctx, "Immediates must be sorted" );
         return FALSE;
      }

      eat_opt_white( &ctx->cur );
      if (*ctx->cur != ']') {
         report_error( ctx, "Expected `]'" );
         return FALSE;
      }

      ctx->cur++;
   }

   if (!eat_white( &ctx->cur )) {
      report_error( ctx, "Syntax error" );
      return FALSE;
   }
   for (type = 0; type < Elements(tgsi_immediate_type_names); ++type) {
      if (str_match_nocase_whole(&ctx->cur, tgsi_immediate_type_names[type]))
         break;
   }
   if (type == Elements(tgsi_immediate_type_names)) {
      report_error( ctx, "Expected immediate type" );
      return FALSE;
   }

   imm = tgsi_default_full_immediate();
   imm.Immediate.NrTokens += 4;
   imm.Immediate.DataType = type;
   parse_immediate_data(ctx, type, imm.u);

   advance = tgsi_build_full_immediate(
      &imm,
      ctx->tokens_cur,
      ctx->header,
      (uint) (ctx->tokens_end - ctx->tokens_cur) );
   if (advance == 0)
      return FALSE;
   ctx->tokens_cur += advance;

   ctx->num_immediates++;

   return TRUE;
}

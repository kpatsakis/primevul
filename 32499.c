parse_register_file_bracket(
   struct translate_ctx *ctx,
   uint *file )
{
   if (!parse_file( &ctx->cur, file )) {
      report_error( ctx, "Unknown register file" );
      return FALSE;
   }
   eat_opt_white( &ctx->cur );
   if (*ctx->cur != '[') {
      report_error( ctx, "Expected `['" );
      return FALSE;
   }
   ctx->cur++;
   return TRUE;
}

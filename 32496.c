parse_register_1d(struct translate_ctx *ctx,
                  uint *file,
                  int *index )
{
   if (!parse_register_file_bracket_index( ctx, file, index ))
      return FALSE;
   eat_opt_white( &ctx->cur );
   if (*ctx->cur != ']') {
      report_error( ctx, "Expected `]'" );
      return FALSE;
   }
   ctx->cur++;
   return TRUE;
}

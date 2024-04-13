static boolean parse_header( struct translate_ctx *ctx )
{
   uint processor;

   if (str_match_nocase_whole( &ctx->cur, "FRAG" ))
      processor = TGSI_PROCESSOR_FRAGMENT;
   else if (str_match_nocase_whole( &ctx->cur, "VERT" ))
      processor = TGSI_PROCESSOR_VERTEX;
   else if (str_match_nocase_whole( &ctx->cur, "GEOM" ))
      processor = TGSI_PROCESSOR_GEOMETRY;
   else if (str_match_nocase_whole( &ctx->cur, "TESS_CTRL" ))
      processor = TGSI_PROCESSOR_TESS_CTRL;
   else if (str_match_nocase_whole( &ctx->cur, "TESS_EVAL" ))
      processor = TGSI_PROCESSOR_TESS_EVAL;
   else if (str_match_nocase_whole( &ctx->cur, "COMP" ))
      processor = TGSI_PROCESSOR_COMPUTE;
   else {
      report_error( ctx, "Unknown header" );
      return FALSE;
   }

   if (ctx->tokens_cur >= ctx->tokens_end)
      return FALSE;
   ctx->header = (struct tgsi_header *) ctx->tokens_cur++;
   *ctx->header = tgsi_build_header();

   if (ctx->tokens_cur >= ctx->tokens_end)
      return FALSE;
   *(struct tgsi_processor *) ctx->tokens_cur++ = tgsi_build_processor( processor, ctx->header );
   ctx->processor = processor;

   return TRUE;
}

parse_opt_register_src_bracket(
   struct translate_ctx *ctx,
   struct parsed_bracket *brackets,
   int *parsed_brackets)
{
   const char *cur = ctx->cur;

   *parsed_brackets = 0;

   eat_opt_white( &cur );
   if (cur[0] == '[') {
      ++cur;
      ctx->cur = cur;

      if (!parse_register_bracket(ctx, brackets))
         return FALSE;

      *parsed_brackets = 1;
   }

   return TRUE;
}

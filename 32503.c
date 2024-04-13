tgsi_text_translate(
   const char *text,
   struct tgsi_token *tokens,
   uint num_tokens )
{
   struct translate_ctx ctx = {0};

   ctx.text = text;
   ctx.cur = text;
   ctx.tokens = tokens;
   ctx.tokens_cur = tokens;
   ctx.tokens_end = tokens + num_tokens;

   if (!translate( &ctx ))
      return FALSE;

   return tgsi_sanity_check( tokens );
}

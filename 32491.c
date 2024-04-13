static boolean parse_label( struct translate_ctx *ctx, uint *val )
{
   const char *cur = ctx->cur;

   if (parse_uint( &cur, val )) {
      eat_opt_white( &cur );
      if (*cur == ':') {
         cur++;
         ctx->cur = cur;
         return TRUE;
      }
   }
   return FALSE;
}

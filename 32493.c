parse_optional_swizzle(
   struct translate_ctx *ctx,
   uint *swizzle,
   boolean *parsed_swizzle,
   int components)
{
   const char *cur = ctx->cur;

   *parsed_swizzle = FALSE;

   eat_opt_white( &cur );
   if (*cur == '.') {
      uint i;

      cur++;
      eat_opt_white( &cur );
      for (i = 0; i < components; i++) {
         if (uprcase( *cur ) == 'X')
            swizzle[i] = TGSI_SWIZZLE_X;
         else if (uprcase( *cur ) == 'Y')
            swizzle[i] = TGSI_SWIZZLE_Y;
         else if (uprcase( *cur ) == 'Z')
            swizzle[i] = TGSI_SWIZZLE_Z;
         else if (uprcase( *cur ) == 'W')
            swizzle[i] = TGSI_SWIZZLE_W;
         else {
	    report_error( ctx, "Expected register swizzle component `x', `y', `z' or `w'" );
	    return FALSE;
         }
         cur++;
      }
      *parsed_swizzle = TRUE;
      ctx->cur = cur;
   }
   return TRUE;
}

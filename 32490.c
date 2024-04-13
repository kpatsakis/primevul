static boolean parse_immediate_data(struct translate_ctx *ctx, unsigned type,
                                    union tgsi_immediate_data *values)
{
   unsigned i;
   int ret;

   eat_opt_white( &ctx->cur );
   if (*ctx->cur != '{') {
      report_error( ctx, "Expected `{'" );
      return FALSE;
   }
   ctx->cur++;
   for (i = 0; i < 4; i++) {
      eat_opt_white( &ctx->cur );
      if (i > 0) {
         if (*ctx->cur != ',') {
            report_error( ctx, "Expected `,'" );
            return FALSE;
         }
         ctx->cur++;
         eat_opt_white( &ctx->cur );
      }

      switch (type) {
      case TGSI_IMM_FLOAT64:
         ret = parse_double(&ctx->cur, &values[i].Uint, &values[i+1].Uint);
         i++;
         break;
      case TGSI_IMM_FLOAT32:
         ret = parse_float(&ctx->cur, &values[i].Float);
         break;
      case TGSI_IMM_UINT32:
         ret = parse_uint(&ctx->cur, &values[i].Uint);
         break;
      case TGSI_IMM_INT32:
         ret = parse_int(&ctx->cur, &values[i].Int);
         break;
      default:
         assert(0);
         ret = FALSE;
         break;
      }

      if (!ret) {
         report_error( ctx, "Expected immediate constant" );
         return FALSE;
      }
   }
   eat_opt_white( &ctx->cur );
   if (*ctx->cur != '}') {
      report_error( ctx, "Expected `}'" );
      return FALSE;
   }
   ctx->cur++;

   return TRUE;
}

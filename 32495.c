static boolean parse_property( struct translate_ctx *ctx )
{
   struct tgsi_full_property prop;
   uint property_name;
   uint values[8];
   uint advance;
   char id[64];

   if (!eat_white( &ctx->cur )) {
      report_error( ctx, "Syntax error" );
      return FALSE;
   }
   if (!parse_identifier( &ctx->cur, id, sizeof(id) )) {
      report_error( ctx, "Syntax error" );
      return FALSE;
   }
   for (property_name = 0; property_name < TGSI_PROPERTY_COUNT;
        ++property_name) {
      if (streq_nocase_uprcase(tgsi_property_names[property_name], id)) {
         break;
      }
   }
   if (property_name >= TGSI_PROPERTY_COUNT) {
      eat_until_eol( &ctx->cur );
      report_error(ctx, "\nError: Unknown property : '%s'\n", id);
      return TRUE;
   }

   eat_opt_white( &ctx->cur );
   switch(property_name) {
   case TGSI_PROPERTY_GS_INPUT_PRIM:
   case TGSI_PROPERTY_GS_OUTPUT_PRIM:
      if (!parse_primitive(&ctx->cur, &values[0] )) {
         report_error( ctx, "Unknown primitive name as property!" );
         return FALSE;
      }
      if (property_name == TGSI_PROPERTY_GS_INPUT_PRIM &&
          ctx->processor == TGSI_PROCESSOR_GEOMETRY) {
         ctx->implied_array_size = u_vertices_per_prim(values[0]);
      }
      break;
   case TGSI_PROPERTY_FS_COORD_ORIGIN:
      if (!parse_fs_coord_origin(&ctx->cur, &values[0] )) {
         report_error( ctx, "Unknown coord origin as property: must be UPPER_LEFT or LOWER_LEFT!" );
         return FALSE;
      }
      break;
   case TGSI_PROPERTY_FS_COORD_PIXEL_CENTER:
      if (!parse_fs_coord_pixel_center(&ctx->cur, &values[0] )) {
         report_error( ctx, "Unknown coord pixel center as property: must be HALF_INTEGER or INTEGER!" );
         return FALSE;
      }
      break;
   case TGSI_PROPERTY_FS_COLOR0_WRITES_ALL_CBUFS:
   default:
      if (!parse_uint(&ctx->cur, &values[0] )) {
         report_error( ctx, "Expected unsigned integer as property!" );
         return FALSE;
      }
   }

   prop = tgsi_default_full_property();
   prop.Property.PropertyName = property_name;
   prop.Property.NrTokens += 1;
   prop.u[0].Data = values[0];

   advance = tgsi_build_full_property(
      &prop,
      ctx->tokens_cur,
      ctx->header,
      (uint) (ctx->tokens_end - ctx->tokens_cur) );
   if (advance == 0)
      return FALSE;
   ctx->tokens_cur += advance;

   return TRUE;
}

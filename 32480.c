static boolean parse_declaration( struct translate_ctx *ctx )
{
   struct tgsi_full_declaration decl;
   uint file;
   struct parsed_dcl_bracket brackets[2];
   int num_brackets;
   uint writemask;
   const char *cur, *cur2;
   uint advance;
   boolean is_vs_input;

   if (!eat_white( &ctx->cur )) {
      report_error( ctx, "Syntax error" );
      return FALSE;
   }
   if (!parse_register_dcl( ctx, &file, brackets, &num_brackets))
      return FALSE;
   if (!parse_opt_writemask( ctx, &writemask ))
      return FALSE;

   decl = tgsi_default_full_declaration();
   decl.Declaration.File = file;
   decl.Declaration.UsageMask = writemask;

   if (num_brackets == 1) {
      decl.Range.First = brackets[0].first;
      decl.Range.Last = brackets[0].last;
   } else {
      decl.Range.First = brackets[1].first;
      decl.Range.Last = brackets[1].last;

      decl.Declaration.Dimension = 1;
      decl.Dim.Index2D = brackets[0].first;
   }

   is_vs_input = (file == TGSI_FILE_INPUT &&
                  ctx->processor == TGSI_PROCESSOR_VERTEX);

   cur = ctx->cur;
   eat_opt_white( &cur );
   if (*cur == ',') {
      cur2 = cur;
      cur2++;
      eat_opt_white( &cur2 );
      if (str_match_nocase_whole( &cur2, "ARRAY" )) {
         int arrayid;
         if (*cur2 != '(') {
            report_error( ctx, "Expected `('" );
            return FALSE;
         }
         cur2++;
         eat_opt_white( &cur2 );
         if (!parse_int( &cur2, &arrayid )) {
            report_error( ctx, "Expected `,'" );
            return FALSE;
         }
         eat_opt_white( &cur2 );
         if (*cur2 != ')') {
            report_error( ctx, "Expected `)'" );
            return FALSE;
         }
         cur2++;
         decl.Declaration.Array = 1;
         decl.Array.ArrayID = arrayid;
         ctx->cur = cur = cur2;
      }
   }

   if (*cur == ',' && !is_vs_input) {
      uint i, j;

      cur++;
      eat_opt_white( &cur );
      if (file == TGSI_FILE_RESOURCE) {
         for (i = 0; i < TGSI_TEXTURE_COUNT; i++) {
            if (str_match_nocase_whole(&cur, tgsi_texture_names[i])) {
               decl.Resource.Resource = i;
               break;
            }
         }
         if (i == TGSI_TEXTURE_COUNT) {
            report_error(ctx, "Expected texture target");
            return FALSE;
         }

         cur2 = cur;
         eat_opt_white(&cur2);
         while (*cur2 == ',') {
            cur2++;
            eat_opt_white(&cur2);
            if (str_match_nocase_whole(&cur2, "RAW")) {
               decl.Resource.Raw = 1;

            } else if (str_match_nocase_whole(&cur2, "WR")) {
               decl.Resource.Writable = 1;

            } else {
               break;
            }
            cur = cur2;
            eat_opt_white(&cur2);
         }

         ctx->cur = cur;

      } else if (file == TGSI_FILE_SAMPLER_VIEW) {
         for (i = 0; i < TGSI_TEXTURE_COUNT; i++) {
            if (str_match_nocase_whole(&cur, tgsi_texture_names[i])) {
               decl.SamplerView.Resource = i;
               break;
            }
         }
         if (i == TGSI_TEXTURE_COUNT) {
            report_error(ctx, "Expected texture target");
            return FALSE;
         }
         eat_opt_white( &cur );
         if (*cur != ',') {
            report_error( ctx, "Expected `,'" );
            return FALSE;
         }
         ++cur;
         eat_opt_white( &cur );
         for (j = 0; j < 4; ++j) {
            for (i = 0; i < TGSI_RETURN_TYPE_COUNT; ++i) {
               if (str_match_nocase_whole(&cur, tgsi_return_type_names[i])) {
                  switch (j) {
                  case 0:
                     decl.SamplerView.ReturnTypeX = i;
                     break;
                  case 1:
                     decl.SamplerView.ReturnTypeY = i;
                     break;
                  case 2:
                     decl.SamplerView.ReturnTypeZ = i;
                     break;
                  case 3:
                     decl.SamplerView.ReturnTypeW = i;
                     break;
                  default:
                     assert(0);
                  }
                  break;
               }
            }
            if (i == TGSI_RETURN_TYPE_COUNT) {
               if (j == 0 || j >  2) {
                  report_error(ctx, "Expected type name");
                  return FALSE;
               }
               break;
            } else {
               cur2 = cur;
               eat_opt_white( &cur2 );
               if (*cur2 == ',') {
                  cur2++;
                  eat_opt_white( &cur2 );
                  cur = cur2;
                  continue;
               } else
                  break;
            }
         }
         if (j < 4) {
            decl.SamplerView.ReturnTypeY =
               decl.SamplerView.ReturnTypeZ =
               decl.SamplerView.ReturnTypeW =
               decl.SamplerView.ReturnTypeX;
         }
         ctx->cur = cur;
      } else {
         if (str_match_nocase_whole(&cur, "LOCAL")) {
            decl.Declaration.Local = 1;
            ctx->cur = cur;
         }

         cur = ctx->cur;
         eat_opt_white( &cur );
         if (*cur == ',') {
            cur++;
            eat_opt_white( &cur );

            for (i = 0; i < TGSI_SEMANTIC_COUNT; i++) {
               if (str_match_nocase_whole(&cur, tgsi_semantic_names[i])) {
                  uint index;

                  cur2 = cur;
                  eat_opt_white( &cur2 );
                  if (*cur2 == '[') {
                     cur2++;
                     eat_opt_white( &cur2 );
                     if (!parse_uint( &cur2, &index )) {
                        report_error( ctx, "Expected literal integer" );
                        return FALSE;
                     }
                     eat_opt_white( &cur2 );
                     if (*cur2 != ']') {
                        report_error( ctx, "Expected `]'" );
                        return FALSE;
                     }
                     cur2++;

                     decl.Semantic.Index = index;

                     cur = cur2;
                  }

                  decl.Declaration.Semantic = 1;
                  decl.Semantic.Name = i;

                  ctx->cur = cur;
                  break;
               }
            }
         }
      }
   }

   cur = ctx->cur;
   eat_opt_white( &cur );
   if (*cur == ',' && !is_vs_input) {
      uint i;

      cur++;
      eat_opt_white( &cur );
      for (i = 0; i < TGSI_INTERPOLATE_COUNT; i++) {
         if (str_match_nocase_whole( &cur, tgsi_interpolate_names[i] )) {
            decl.Declaration.Interpolate = 1;
            decl.Interp.Interpolate = i;

            ctx->cur = cur;
            break;
         }
      }
      if (i == TGSI_INTERPOLATE_COUNT) {
         report_error( ctx, "Expected semantic or interpolate attribute" );
         return FALSE;
      }
   }

   cur = ctx->cur;
   eat_opt_white( &cur );
   if (*cur == ',' && !is_vs_input) {
      uint i;

      cur++;
      eat_opt_white( &cur );
      for (i = 0; i < TGSI_INTERPOLATE_LOC_COUNT; i++) {
         if (str_match_nocase_whole( &cur, tgsi_interpolate_locations[i] )) {
            decl.Interp.Location = i;

            ctx->cur = cur;
            break;
         }
      }
   }

   advance = tgsi_build_full_declaration(
      &decl,
      ctx->tokens_cur,
      ctx->header,
      (uint) (ctx->tokens_end - ctx->tokens_cur) );

   if (advance == 0)
      return FALSE;
   ctx->tokens_cur += advance;

   return TRUE;
}

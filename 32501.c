parse_texoffset_operand(
   struct translate_ctx *ctx,
   struct tgsi_texture_offset *src )
{
   uint file;
   uint swizzle[3];
   boolean parsed_swizzle;
   struct parsed_bracket bracket;

   if (!parse_register_src(ctx, &file, &bracket))
      return FALSE;

   src->File = file;
   src->Index = bracket.index;

   /* Parse optional swizzle.
    */
   if (parse_optional_swizzle( ctx, swizzle, &parsed_swizzle, 3 )) {
      if (parsed_swizzle) {
         src->SwizzleX = swizzle[0];
         src->SwizzleY = swizzle[1];
         src->SwizzleZ = swizzle[2];
      }
   }

   return TRUE;
}

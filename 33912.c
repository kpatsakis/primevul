  cf2_getScaleAndHintFlag( CFF_Decoder*  decoder,
                           CF2_Fixed*    x_scale,
                           CF2_Fixed*    y_scale,
                           FT_Bool*      hinted,
                           FT_Bool*      scaled )
  {
    FT_ASSERT( decoder && decoder->builder.glyph );

    /* note: FreeType scale includes a factor of 64 */
    *hinted = decoder->builder.glyph->hint;
    *scaled = decoder->builder.glyph->scaled;

    if ( *hinted )
    {
      *x_scale = FT_DivFix( decoder->builder.glyph->x_scale,
                            cf2_intToFixed( 64 ) );
      *y_scale = FT_DivFix( decoder->builder.glyph->y_scale,
                            cf2_intToFixed( 64 ) );
    }
    else
    {
      /* for unhinted outlines, `cff_slot_load' does the scaling, */
      /* thus render at `unity' scale                             */

      *x_scale = 0x0400;   /* 1/64 as 16.16 */
      *y_scale = 0x0400;
    }
  }

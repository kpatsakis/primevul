  cf2_setGlyphWidth( CF2_Outline  outline,
                     CF2_Fixed    width )
  {
    CFF_Decoder*  decoder = outline->decoder;


    FT_ASSERT( decoder );

    decoder->glyph_width = cf2_fixedToInt( width );
  }

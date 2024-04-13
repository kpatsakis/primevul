  t1_decoder_parse_glyph( T1_Decoder  decoder,
                          FT_UInt     glyph )
  {
    return decoder->parse_callback( decoder, glyph );
  }

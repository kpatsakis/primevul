  cf2_getStdHW( CFF_Decoder*  decoder )
  {
    FT_ASSERT( decoder && decoder->current_subfont );

    return cf2_intToFixed(
             decoder->current_subfont->private_dict.standard_width );
  }

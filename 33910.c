  cf2_getOtherBlues( CFF_Decoder*  decoder,
                     size_t*       count,
                     FT_Pos*      *data )
  {
    FT_ASSERT( decoder && decoder->current_subfont );

    *count = decoder->current_subfont->private_dict.num_other_blues;
    *data  = (FT_Pos*)
               &decoder->current_subfont->private_dict.other_blues;
  }

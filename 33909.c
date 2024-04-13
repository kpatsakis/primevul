  cf2_getLanguageGroup( CFF_Decoder*  decoder )
  {
    FT_ASSERT( decoder && decoder->current_subfont );

    return decoder->current_subfont->private_dict.language_group;
  }

  cf2_getUnitsPerEm( CFF_Decoder*  decoder )
  {
    FT_ASSERT( decoder && decoder->builder.face );
    FT_ASSERT( decoder->builder.face->root.units_per_EM );

    return decoder->builder.face->root.units_per_EM;
  }

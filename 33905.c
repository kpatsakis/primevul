  cf2_freeSeacComponent( CFF_Decoder*  decoder,
                         CF2_Buffer    buf )
  {
    FT_ASSERT( decoder );

    cff_free_glyph_data( decoder->builder.face,
                         (FT_Byte**)&buf->start,
                         (FT_ULong)( buf->end - buf->start ) );
  }

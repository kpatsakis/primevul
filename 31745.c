  tt_sbit_decoder_load_png( TT_SBitDecoder  decoder,
                            FT_Byte*        p,
                            FT_Byte*        limit,
                            FT_Int          x_pos,
                            FT_Int          y_pos )
  {
    FT_Error  error = FT_Err_Ok;
    FT_ULong  png_len;


    if ( limit - p < 4 )
    {
      FT_TRACE1(( "tt_sbit_decoder_load_png: broken bitmap\n" ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    png_len = FT_NEXT_ULONG( p );
    if ( (FT_ULong)( limit - p ) < png_len )
    {
      FT_TRACE1(( "tt_sbit_decoder_load_png: broken bitmap\n" ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    error = Load_SBit_Png( decoder->face->root.glyph,
                           x_pos,
                           y_pos,
                           decoder->bit_depth,
                           decoder->metrics,
                           decoder->stream->memory,
                           p,
                           png_len,
                           FALSE );

  Exit:
    if ( !error )
      FT_TRACE3(( "tt_sbit_decoder_load_png: loaded\n" ));
    return error;
  }

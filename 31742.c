  tt_sbit_decoder_load_compound( TT_SBitDecoder  decoder,
                                 FT_Byte*        p,
                                 FT_Byte*        limit,
                                 FT_Int          x_pos,
                                 FT_Int          y_pos )
  {
    FT_Error  error = FT_Err_Ok;
    FT_UInt   num_components, nn;

    FT_Char  horiBearingX = (FT_Char)decoder->metrics->horiBearingX;
    FT_Char  horiBearingY = (FT_Char)decoder->metrics->horiBearingY;
    FT_Byte  horiAdvance  = (FT_Byte)decoder->metrics->horiAdvance;
    FT_Char  vertBearingX = (FT_Char)decoder->metrics->vertBearingX;
    FT_Char  vertBearingY = (FT_Char)decoder->metrics->vertBearingY;
    FT_Byte  vertAdvance  = (FT_Byte)decoder->metrics->vertAdvance;


    if ( p + 2 > limit )
      goto Fail;

    num_components = FT_NEXT_USHORT( p );
    if ( p + 4 * num_components > limit )
    {
      FT_TRACE1(( "tt_sbit_decoder_load_compound: broken table\n" ));
      goto Fail;
    }

    FT_TRACE3(( "tt_sbit_decoder_load_compound: loading %d components\n",
                num_components ));

    for ( nn = 0; nn < num_components; nn++ )
    {
      FT_UInt  gindex = FT_NEXT_USHORT( p );
      FT_Byte  dx     = FT_NEXT_BYTE( p );
      FT_Byte  dy     = FT_NEXT_BYTE( p );


      /* NB: a recursive call */
      error = tt_sbit_decoder_load_image( decoder, gindex,
                                          x_pos + dx, y_pos + dy );
      if ( error )
        break;
    }

    FT_TRACE3(( "tt_sbit_decoder_load_compound: done\n" ));

    decoder->metrics->horiBearingX = horiBearingX;
    decoder->metrics->horiBearingY = horiBearingY;
    decoder->metrics->horiAdvance  = horiAdvance;
    decoder->metrics->vertBearingX = vertBearingX;
    decoder->metrics->vertBearingY = vertBearingY;
    decoder->metrics->vertAdvance  = vertAdvance;
    decoder->metrics->width        = (FT_Byte)decoder->bitmap->width;
    decoder->metrics->height       = (FT_Byte)decoder->bitmap->rows;

  Exit:
    return error;

  Fail:
    error = FT_THROW( Invalid_File_Format );
    goto Exit;
  }

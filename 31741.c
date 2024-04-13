  tt_sbit_decoder_load_byte_aligned( TT_SBitDecoder  decoder,
                                     FT_Byte*        p,
                                     FT_Byte*        limit,
                                     FT_Int          x_pos,
                                     FT_Int          y_pos )
  {
    FT_Error    error = FT_Err_Ok;
    FT_Byte*    line;
    FT_Int      bit_height, bit_width, pitch, width, height, line_bits, h;
    FT_Bitmap*  bitmap;


    /* check that we can write the glyph into the bitmap */
    bitmap     = decoder->bitmap;
    bit_width  = bitmap->width;
    bit_height = bitmap->rows;
    pitch      = bitmap->pitch;
    line       = bitmap->buffer;

    width  = decoder->metrics->width;
    height = decoder->metrics->height;

    line_bits = width * decoder->bit_depth;

    if ( x_pos < 0 || x_pos + width > bit_width   ||
         y_pos < 0 || y_pos + height > bit_height )
    {
      FT_TRACE1(( "tt_sbit_decoder_load_byte_aligned:"
                  " invalid bitmap dimensions\n" ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    if ( p + ( ( line_bits + 7 ) >> 3 ) * height > limit )
    {
      FT_TRACE1(( "tt_sbit_decoder_load_byte_aligned: broken bitmap\n" ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    /* now do the blit */
    line  += y_pos * pitch + ( x_pos >> 3 );
    x_pos &= 7;

    if ( x_pos == 0 )  /* the easy one */
    {
      for ( h = height; h > 0; h--, line += pitch )
      {
        FT_Byte*  pwrite = line;
        FT_Int    w;


        for ( w = line_bits; w >= 8; w -= 8 )
        {
          pwrite[0] = (FT_Byte)( pwrite[0] | *p++ );
          pwrite   += 1;
        }

        if ( w > 0 )
          pwrite[0] = (FT_Byte)( pwrite[0] | ( *p++ & ( 0xFF00U >> w ) ) );
      }
    }
    else  /* x_pos > 0 */
    {
      for ( h = height; h > 0; h--, line += pitch )
      {
        FT_Byte*  pwrite = line;
        FT_Int    w;
        FT_UInt   wval = 0;


        for ( w = line_bits; w >= 8; w -= 8 )
        {
          wval       = (FT_UInt)( wval | *p++ );
          pwrite[0]  = (FT_Byte)( pwrite[0] | ( wval >> x_pos ) );
          pwrite    += 1;
          wval     <<= 8;
        }

        if ( w > 0 )
          wval = (FT_UInt)( wval | ( *p++ & ( 0xFF00U >> w ) ) );

        /* all bits read and there are `x_pos + w' bits to be written */

        pwrite[0] = (FT_Byte)( pwrite[0] | ( wval >> x_pos ) );

        if ( x_pos + w > 8 )
        {
          pwrite++;
          wval     <<= 8;
          pwrite[0]  = (FT_Byte)( pwrite[0] | ( wval >> x_pos ) );
        }
      }
    }

  Exit:
    if ( !error )
      FT_TRACE3(( "tt_sbit_decoder_load_byte_aligned: loaded\n" ));
    return error;
  }

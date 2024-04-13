  tt_sbit_decoder_load_bitmap( TT_SBitDecoder  decoder,
                               FT_UInt         glyph_format,
                               FT_ULong        glyph_start,
                               FT_ULong        glyph_size,
                               FT_Int          x_pos,
                               FT_Int          y_pos )
  {
    FT_Error   error;
    FT_Stream  stream = decoder->stream;
    FT_Byte*   p;
    FT_Byte*   p_limit;
    FT_Byte*   data;


    /* seek into the EBDT table now */
    if ( glyph_start + glyph_size > decoder->ebdt_size )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    if ( FT_STREAM_SEEK( decoder->ebdt_start + glyph_start ) ||
         FT_FRAME_EXTRACT( glyph_size, data )                )
      goto Exit;

    p       = data;
    p_limit = p + glyph_size;

    /* read the data, depending on the glyph format */
    switch ( glyph_format )
    {
    case 1:
    case 2:
    case 8:
    case 17:
      error = tt_sbit_decoder_load_metrics( decoder, &p, p_limit, 0 );
      break;

    case 6:
    case 7:
    case 9:
    case 18:
      error = tt_sbit_decoder_load_metrics( decoder, &p, p_limit, 1 );
      break;

    default:
      error = FT_Err_Ok;
    }

    if ( error )
      goto Fail;

    {
      TT_SBitDecoder_LoadFunc  loader;


      switch ( glyph_format )
      {
      case 1:
      case 6:
        loader = tt_sbit_decoder_load_byte_aligned;
        break;

      case 2:
      case 7:
        {
          /* Don't trust `glyph_format'.  For example, Apple's main Korean */
          /* system font, `AppleMyungJo.ttf' (version 7.0d2e6), uses glyph */
          /* format 7, but the data is format 6.  We check whether we have */
          /* an excessive number of bytes in the image: If it is equal to  */
          /* the value for a byte-aligned glyph, use the other loading     */
          /* routine.                                                      */
          /*                                                               */
          /* Note that for some (width,height) combinations, where the     */
          /* width is not a multiple of 8, the sizes for bit- and          */
          /* byte-aligned data are equal, for example (7,7) or (15,6).  We */
          /* then prefer what `glyph_format' specifies.                    */

          FT_UInt  width  = decoder->metrics->width;
          FT_UInt  height = decoder->metrics->height;

          FT_UInt  bit_size  = ( width * height + 7 ) >> 3;
          FT_UInt  byte_size = height * ( ( width + 7 ) >> 3 );


          if ( bit_size < byte_size                  &&
               byte_size == (FT_UInt)( p_limit - p ) )
            loader = tt_sbit_decoder_load_byte_aligned;
          else
            loader = tt_sbit_decoder_load_bit_aligned;
        }
        break;

      case 5:
        loader = tt_sbit_decoder_load_bit_aligned;
        break;

      case 8:
        if ( p + 1 > p_limit )
          goto Fail;

        p += 1;  /* skip padding */
        /* fall-through */

      case 9:
        loader = tt_sbit_decoder_load_compound;
        break;

      case 17: /* small metrics, PNG image data   */
      case 18: /* big metrics, PNG image data     */
      case 19: /* metrics in EBLC, PNG image data */
#ifdef FT_CONFIG_OPTION_USE_PNG
        loader = tt_sbit_decoder_load_png;
        break;
#else
        error = FT_THROW( Unimplemented_Feature );
        goto Fail;
#endif /* FT_CONFIG_OPTION_USE_PNG */

      default:
        error = FT_THROW( Invalid_Table );
        goto Fail;
      }

      if ( !decoder->bitmap_allocated )
      {
        error = tt_sbit_decoder_alloc_bitmap( decoder );
        if ( error )
          goto Fail;
      }

      error = loader( decoder, p, p_limit, x_pos, y_pos );
    }

  Fail:
    FT_FRAME_RELEASE( data );

  Exit:
    return error;
  }

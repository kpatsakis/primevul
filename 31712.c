  pcf_get_bitmaps( FT_Stream  stream,
                   PCF_Face   face )
  {
    FT_Error   error;
    FT_Memory  memory  = FT_FACE( face )->memory;
    FT_Long*   offsets = NULL;
    FT_Long    bitmapSizes[GLYPHPADOPTIONS];
    FT_ULong   format, size;
    FT_ULong   nbitmaps, i, sizebitmaps = 0;


    error = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_BITMAPS,
                                    &format,
                                    &size );
    if ( error )
      return error;

    error = FT_Stream_EnterFrame( stream, 8 );
    if ( error )
      return error;

    format = FT_GET_ULONG_LE();
    if ( PCF_BYTE_ORDER( format ) == MSBFirst )
      nbitmaps  = FT_GET_ULONG();
    else
      nbitmaps  = FT_GET_ULONG_LE();

    FT_Stream_ExitFrame( stream );

    if ( !PCF_FORMAT_MATCH( format, PCF_DEFAULT_FORMAT ) )
      return FT_THROW( Invalid_File_Format );

    FT_TRACE4(( "pcf_get_bitmaps:\n" ));

    FT_TRACE4(( "  number of bitmaps: %d\n", nbitmaps ));

    /* XXX: PCF_Face->nmetrics is singed FT_Long, see pcf.h */
    if ( face->nmetrics < 0 || nbitmaps != ( FT_ULong )face->nmetrics )
      return FT_THROW( Invalid_File_Format );

    if ( FT_NEW_ARRAY( offsets, nbitmaps ) )
      return error;

    for ( i = 0; i < nbitmaps; i++ )
    {
      if ( PCF_BYTE_ORDER( format ) == MSBFirst )
        (void)FT_READ_LONG( offsets[i] );
      else
        (void)FT_READ_LONG_LE( offsets[i] );

      FT_TRACE5(( "  bitmap %d: offset %ld (0x%lX)\n",
                  i, offsets[i], offsets[i] ));
    }
    if ( error )
      goto Bail;

    for ( i = 0; i < GLYPHPADOPTIONS; i++ )
    {
      if ( PCF_BYTE_ORDER( format ) == MSBFirst )
        (void)FT_READ_LONG( bitmapSizes[i] );
      else
        (void)FT_READ_LONG_LE( bitmapSizes[i] );
      if ( error )
        goto Bail;

      sizebitmaps = bitmapSizes[PCF_GLYPH_PAD_INDEX( format )];

      FT_TRACE4(( "  padding %d implies a size of %ld\n", i, bitmapSizes[i] ));
    }

    FT_TRACE4(( "  %d bitmaps, padding index %ld\n",
                nbitmaps,
                PCF_GLYPH_PAD_INDEX( format ) ));
    FT_TRACE4(( "  bitmap size = %d\n", sizebitmaps ));

    FT_UNUSED( sizebitmaps );       /* only used for debugging */

    for ( i = 0; i < nbitmaps; i++ )
    {
      /* rough estimate */
      if ( ( offsets[i] < 0 )              ||
           ( (FT_ULong)offsets[i] > size ) )
      {
        FT_TRACE0(( "pcf_get_bitmaps:"
                    " invalid offset to bitmap data of glyph %d\n", i ));
      }
      else
        face->metrics[i].bits = stream->pos + offsets[i];
    }

    face->bitmapsFormat = format;

  Bail:
    FT_FREE( offsets );
    return error;
  }

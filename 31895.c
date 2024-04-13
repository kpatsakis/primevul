  sfnt_open_font( FT_Stream  stream,
                  TT_Face    face )
  {
    FT_Memory  memory = stream->memory;
    FT_Error   error;
    FT_ULong   tag, offset;

    static const FT_Frame_Field  ttc_header_fields[] =
    {
#undef  FT_STRUCTURE
#define FT_STRUCTURE  TTC_HeaderRec

      FT_FRAME_START( 8 ),
        FT_FRAME_LONG( version ),
        FT_FRAME_LONG( count   ),  /* this is ULong in the specs */
      FT_FRAME_END
    };


    face->ttc_header.tag     = 0;
    face->ttc_header.version = 0;
    face->ttc_header.count   = 0;

  retry:
    offset = FT_STREAM_POS();

    if ( FT_READ_ULONG( tag ) )
      return error;

    if ( tag == TTAG_wOFF )
    {
      FT_TRACE2(( "sfnt_open_font: file is a WOFF; synthesizing SFNT\n" ));

      if ( FT_STREAM_SEEK( offset ) )
        return error;

      error = woff_open_font( stream, face );
      if ( error )
        return error;

      /* Swap out stream and retry! */
      stream = face->root.stream;
      goto retry;
    }

    if ( tag != 0x00010000UL &&
         tag != TTAG_ttcf    &&
         tag != TTAG_OTTO    &&
         tag != TTAG_true    &&
         tag != TTAG_typ1    &&
         tag != 0x00020000UL )
    {
      FT_TRACE2(( "  not a font using the SFNT container format\n" ));
      return FT_THROW( Unknown_File_Format );
    }

    face->ttc_header.tag = TTAG_ttcf;

    if ( tag == TTAG_ttcf )
    {
      FT_Int  n;


      FT_TRACE3(( "sfnt_open_font: file is a collection\n" ));

      if ( FT_STREAM_READ_FIELDS( ttc_header_fields, &face->ttc_header ) )
        return error;

      FT_TRACE3(( "                with %ld subfonts\n",
                  face->ttc_header.count ));

      if ( face->ttc_header.count == 0 )
        return FT_THROW( Invalid_Table );

      /* a rough size estimate: let's conservatively assume that there   */
      /* is just a single table info in each subfont header (12 + 16*1 = */
      /* 28 bytes), thus we have (at least) `12 + 4*count' bytes for the */
      /* size of the TTC header plus `28*count' bytes for all subfont    */
      /* headers                                                         */
      if ( (FT_ULong)face->ttc_header.count > stream->size / ( 28 + 4 ) )
        return FT_THROW( Array_Too_Large );

      /* now read the offsets of each font in the file */
      if ( FT_NEW_ARRAY( face->ttc_header.offsets, face->ttc_header.count ) )
        return error;

      if ( FT_FRAME_ENTER( face->ttc_header.count * 4L ) )
        return error;

      for ( n = 0; n < face->ttc_header.count; n++ )
        face->ttc_header.offsets[n] = FT_GET_ULONG();

      FT_FRAME_EXIT();
    }
    else
    {
      FT_TRACE3(( "sfnt_open_font: synthesize TTC\n" ));

      face->ttc_header.version = 1 << 16;
      face->ttc_header.count   = 1;

      if ( FT_NEW( face->ttc_header.offsets ) )
        return error;

      face->ttc_header.offsets[0] = offset;
    }

    return error;
  }

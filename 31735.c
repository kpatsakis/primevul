  tt_face_load_sbix_image( TT_Face              face,
                           FT_ULong             strike_index,
                           FT_UInt              glyph_index,
                           FT_Stream            stream,
                           FT_Bitmap           *map,
                           TT_SBit_MetricsRec  *metrics )
  {
    FT_UInt   sbix_pos, strike_offset, glyph_start, glyph_end;
    FT_ULong  table_size;
    FT_Int    originOffsetX, originOffsetY;
    FT_Tag    graphicType;
    FT_Int    recurse_depth = 0;

    FT_Error  error;
    FT_Byte*  p;

    FT_UNUSED( map );


    metrics->width  = 0;
    metrics->height = 0;

    p = face->sbit_table + 8 + 4 * strike_index;
    strike_offset = FT_NEXT_ULONG( p );

    error = face->goto_table( face, TTAG_sbix, stream, &table_size );
    if ( error )
      return error;
    sbix_pos = FT_STREAM_POS();

  retry:
    if ( glyph_index > (FT_UInt)face->root.num_glyphs )
      return FT_THROW( Invalid_Argument );

    if ( strike_offset >= table_size                          ||
         table_size - strike_offset < 4 + glyph_index * 4 + 8 )
      return FT_THROW( Invalid_File_Format );

    if ( FT_STREAM_SEEK( sbix_pos + strike_offset + 4 + glyph_index * 4 ) ||
         FT_FRAME_ENTER( 8 )                                              )
      return error;

    glyph_start = FT_GET_ULONG();
    glyph_end   = FT_GET_ULONG();

    FT_FRAME_EXIT();

    if ( glyph_start == glyph_end )
      return FT_THROW( Invalid_Argument );
    if ( glyph_start > glyph_end                ||
         glyph_end - glyph_start < 8            ||
         table_size - strike_offset < glyph_end )
      return FT_THROW( Invalid_File_Format );

    if ( FT_STREAM_SEEK( sbix_pos + strike_offset + glyph_start ) ||
         FT_FRAME_ENTER( glyph_end - glyph_start )                )
      return error;

    originOffsetX = FT_GET_SHORT();
    originOffsetY = FT_GET_SHORT();

    graphicType = FT_GET_TAG4();

    switch ( graphicType )
    {
    case FT_MAKE_TAG( 'd', 'u', 'p', 'e' ):
      if ( recurse_depth < 4 )
      {
        glyph_index = FT_GET_USHORT();
        FT_FRAME_EXIT();
        recurse_depth++;
        goto retry;
      }
      error = FT_THROW( Invalid_File_Format );
      break;

    case FT_MAKE_TAG( 'p', 'n', 'g', ' ' ):
#ifdef FT_CONFIG_OPTION_USE_PNG
      error = Load_SBit_Png( face->root.glyph,
                             0,
                             0,
                             32,
                             metrics,
                             stream->memory,
                             stream->cursor,
                             glyph_end - glyph_start - 8,
                             TRUE );
#else
      error = FT_THROW( Unimplemented_Feature );
#endif
      break;

    case FT_MAKE_TAG( 'j', 'p', 'g', ' ' ):
    case FT_MAKE_TAG( 't', 'i', 'f', 'f' ):
    case FT_MAKE_TAG( 'r', 'g', 'b', 'l' ): /* used on iOS 7.1 */
      error = FT_THROW( Unknown_File_Format );
      break;

    default:
      error = FT_THROW( Unimplemented_Feature );
      break;
    }

    FT_FRAME_EXIT();

    if ( !error )
    {
      FT_Short   abearing;
      FT_UShort  aadvance;


      tt_face_get_metrics( face, FALSE, glyph_index, &abearing, &aadvance );

      metrics->horiBearingX = (FT_Short)originOffsetX;
      metrics->horiBearingY = (FT_Short)( -originOffsetY + metrics->height );
      metrics->horiAdvance  = (FT_Short)( aadvance *
                                          face->root.size->metrics.x_ppem /
                                          face->header.Units_Per_EM );
    }

    return error;
  }

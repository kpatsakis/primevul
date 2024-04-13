  tt_face_load_strike_metrics( TT_Face           face,
                               FT_ULong          strike_index,
                               FT_Size_Metrics*  metrics )
  {
    if ( strike_index >= (FT_ULong)face->sbit_num_strikes )
      return FT_THROW( Invalid_Argument );

    switch ( (FT_UInt)face->sbit_table_type )
    {
    case TT_SBIT_TABLE_TYPE_EBLC:
    case TT_SBIT_TABLE_TYPE_CBLC:
      {
        FT_Byte*  strike;


        strike = face->sbit_table + 8 + strike_index * 48;

        metrics->x_ppem = (FT_UShort)strike[44];
        metrics->y_ppem = (FT_UShort)strike[45];

        metrics->ascender  = (FT_Char)strike[16] << 6;  /* hori.ascender  */
        metrics->descender = (FT_Char)strike[17] << 6;  /* hori.descender */
        metrics->height    = metrics->ascender - metrics->descender;

        /* Is this correct? */
        metrics->max_advance = ( (FT_Char)strike[22] + /* min_origin_SB  */
                                          strike[18] + /* max_width      */
                                 (FT_Char)strike[23]   /* min_advance_SB */
                                                     ) << 6;
        return FT_Err_Ok;
      }

    case TT_SBIT_TABLE_TYPE_SBIX:
      {
        FT_Stream       stream = face->root.stream;
        FT_UInt         offset, upem;
        FT_UShort       ppem, resolution;
        TT_HoriHeader  *hori;
        FT_ULong        table_size;

        FT_Error  error;
        FT_Byte*  p;


        p      = face->sbit_table + 8 + 4 * strike_index;
        offset = FT_NEXT_ULONG( p );

        error = face->goto_table( face, TTAG_sbix, stream, &table_size );
        if ( error )
          return error;

        if ( offset + 4  > table_size )
          return FT_THROW( Invalid_File_Format );

        if ( FT_STREAM_SEEK( FT_STREAM_POS() + offset ) ||
             FT_FRAME_ENTER( 4 )                        )
          return error;

        ppem       = FT_GET_USHORT();
        resolution = FT_GET_USHORT();

        FT_UNUSED( resolution ); /* What to do with this? */

        FT_FRAME_EXIT();

        upem = face->header.Units_Per_EM;
        hori = &face->horizontal;

        metrics->x_ppem = ppem;
        metrics->y_ppem = ppem;

        metrics->ascender    = ppem * hori->Ascender * 64 / upem;
        metrics->descender   = ppem * hori->Descender * 64 / upem;
        metrics->height      = ppem * ( hori->Ascender -
                                        hori->Descender +
                                        hori->Line_Gap ) * 64 / upem;
        metrics->max_advance = ppem * hori->advance_Width_Max * 64 / upem;

        return error;
      }

    default:
      return FT_THROW( Unknown_File_Format );
    }
  }

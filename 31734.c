  tt_face_load_sbit_image( TT_Face              face,
                           FT_ULong             strike_index,
                           FT_UInt              glyph_index,
                           FT_UInt              load_flags,
                           FT_Stream            stream,
                           FT_Bitmap           *map,
                           TT_SBit_MetricsRec  *metrics )
  {
    FT_Error  error = FT_Err_Ok;


    switch ( (FT_UInt)face->sbit_table_type )
    {
    case TT_SBIT_TABLE_TYPE_EBLC:
    case TT_SBIT_TABLE_TYPE_CBLC:
      {
        TT_SBitDecoderRec  decoder[1];


        error = tt_sbit_decoder_init( decoder, face, strike_index, metrics );
        if ( !error )
        {
          error = tt_sbit_decoder_load_image( decoder,
                                              glyph_index,
                                              0,
                                              0 );
          tt_sbit_decoder_done( decoder );
        }
      }
      break;

    case TT_SBIT_TABLE_TYPE_SBIX:
      error = tt_face_load_sbix_image( face,
                                       strike_index,
                                       glyph_index,
                                       stream,
                                       map,
                                       metrics );
      break;

    default:
      error = FT_THROW( Unknown_File_Format );
      break;
    }

    /* Flatten color bitmaps if color was not requested. */
    if ( !error                                &&
         !( load_flags & FT_LOAD_COLOR )       &&
         map->pixel_mode == FT_PIXEL_MODE_BGRA )
    {
      FT_Bitmap   new_map;
      FT_Library  library = face->root.glyph->library;


      FT_Bitmap_New( &new_map );

      /* Convert to 8bit grayscale. */
      error = FT_Bitmap_Convert( library, map, &new_map, 1 );
      if ( error )
        FT_Bitmap_Done( library, &new_map );
      else
      {
        map->pixel_mode = new_map.pixel_mode;
        map->pitch      = new_map.pitch;
        map->num_grays  = new_map.num_grays;

        ft_glyphslot_set_bitmap( face->root.glyph, new_map.buffer );
        face->root.glyph->internal->flags |= FT_GLYPH_OWN_BITMAP;
      }
    }

    return error;
  }

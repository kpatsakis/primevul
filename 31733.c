  tt_face_load_sbit( TT_Face    face,
                     FT_Stream  stream )
  {
    FT_Error  error;
    FT_ULong  table_size;


    face->sbit_table       = NULL;
    face->sbit_table_size  = 0;
    face->sbit_table_type  = TT_SBIT_TABLE_TYPE_NONE;
    face->sbit_num_strikes = 0;

    error = face->goto_table( face, TTAG_CBLC, stream, &table_size );
    if ( !error )
      face->sbit_table_type = TT_SBIT_TABLE_TYPE_CBLC;
    else
    {
      error = face->goto_table( face, TTAG_EBLC, stream, &table_size );
      if ( error )
        error = face->goto_table( face, TTAG_bloc, stream, &table_size );
      if ( !error )
        face->sbit_table_type = TT_SBIT_TABLE_TYPE_EBLC;
    }

    if ( error )
    {
      error = face->goto_table( face, TTAG_sbix, stream, &table_size );
      if ( !error )
        face->sbit_table_type = TT_SBIT_TABLE_TYPE_SBIX;
    }
    if ( error )
      goto Exit;

    if ( table_size < 8 )
    {
      FT_ERROR(( "tt_face_load_sbit_strikes: table too short\n" ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    switch ( (FT_UInt)face->sbit_table_type )
    {
    case TT_SBIT_TABLE_TYPE_EBLC:
    case TT_SBIT_TABLE_TYPE_CBLC:
      {
        FT_Byte*  p;
        FT_Fixed  version;
        FT_ULong  num_strikes;
        FT_UInt   count;


        if ( FT_FRAME_EXTRACT( table_size, face->sbit_table ) )
          goto Exit;

        face->sbit_table_size = table_size;

        p = face->sbit_table;

        version     = FT_NEXT_ULONG( p );
        num_strikes = FT_NEXT_ULONG( p );

        if ( ( version & 0xFFFF0000UL ) != 0x00020000UL )
        {
          error = FT_THROW( Unknown_File_Format );
          goto Exit;
        }

        if ( num_strikes >= 0x10000UL )
        {
          error = FT_THROW( Invalid_File_Format );
          goto Exit;
        }

        /*
         *  Count the number of strikes available in the table.  We are a bit
         *  paranoid there and don't trust the data.
         */
        count = (FT_UInt)num_strikes;
        if ( 8 + 48UL * count > table_size )
          count = (FT_UInt)( ( table_size - 8 ) / 48 );

        face->sbit_num_strikes = count;
      }
      break;

    case TT_SBIT_TABLE_TYPE_SBIX:
      {
        FT_UShort  version;
        FT_UShort  flags;
        FT_ULong   num_strikes;
        FT_UInt    count;


        if ( FT_FRAME_ENTER( 8 ) )
          goto Exit;

        version     = FT_GET_USHORT();
        flags       = FT_GET_USHORT();
        num_strikes = FT_GET_ULONG();

        FT_FRAME_EXIT();

        if ( version < 1 )
        {
          error = FT_THROW( Unknown_File_Format );
          goto Exit;
        }

        /* Bit 0 must always be `1'.                            */
        /* Bit 1 controls the overlay of bitmaps with outlines. */
        /* All other bits should be zero.                       */
        if ( !( flags == 1 || flags == 3 ) ||
             num_strikes >= 0x10000UL      )
        {
          error = FT_THROW( Invalid_File_Format );
          goto Exit;
        }

        /* we currently don't support bit 1; however, it is better to */
        /* draw at least something...                                 */
        if ( flags == 3 )
          FT_TRACE1(( "tt_face_load_sbit_strikes:"
                      " sbix overlay not supported yet\n"
                      "                          "
                      " expect bad rendering results\n" ));

        /*
         *  Count the number of strikes available in the table.  We are a bit
         *  paranoid there and don't trust the data.
         */
        count = (FT_UInt)num_strikes;
        if ( 8 + 4UL * count > table_size )
          count = (FT_UInt)( ( table_size - 8 ) / 4 );

        if ( FT_STREAM_SEEK( FT_STREAM_POS() - 8 ) )
          goto Exit;

        face->sbit_table_size = 8 + count * 4;
        if ( FT_FRAME_EXTRACT( face->sbit_table_size, face->sbit_table ) )
          goto Exit;

        face->sbit_num_strikes = count;
      }
      break;

    default:
      error = FT_THROW( Unknown_File_Format );
      break;
    }

    if ( !error )
      FT_TRACE3(( "sbit_num_strikes: %u\n", face->sbit_num_strikes ));

    return FT_Err_Ok;

  Exit:
    if ( error )
    {
      if ( face->sbit_table )
        FT_FRAME_RELEASE( face->sbit_table );
      face->sbit_table_size = 0;
      face->sbit_table_type = TT_SBIT_TABLE_TYPE_NONE;
    }

    return error;
  }

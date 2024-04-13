  cff_index_load_offsets( CFF_Index  idx )
  {
    FT_Error   error  = CFF_Err_Ok;
    FT_Stream  stream = idx->stream;
    FT_Memory  memory = stream->memory;


    if ( idx->count > 0 && idx->offsets == NULL )
    {
      FT_Byte    offsize = idx->off_size;
      FT_ULong   data_size;
      FT_Byte*   p;
      FT_Byte*   p_end;
      FT_ULong*  poff;


      data_size = (FT_ULong)( idx->count + 1 ) * offsize;

      if ( FT_NEW_ARRAY( idx->offsets, idx->count + 1 ) ||
           FT_STREAM_SEEK( idx->start + 3 )             ||
           FT_FRAME_ENTER( data_size )                  )
        goto Exit;

      poff   = idx->offsets;
      p      = (FT_Byte*)stream->cursor;
      p_end  = p + data_size;

      switch ( offsize )
      {
      case 1:
        for ( ; p < p_end; p++, poff++ )
          poff[0] = p[0];
        break;

      case 2:
        for ( ; p < p_end; p += 2, poff++ )
          poff[0] = FT_PEEK_USHORT( p );
        break;

      case 3:
        for ( ; p < p_end; p += 3, poff++ )
          poff[0] = FT_PEEK_OFF3( p );
        break;

      default:
        for ( ; p < p_end; p += 4, poff++ )
          poff[0] = FT_PEEK_ULONG( p );
      }

      FT_FRAME_EXIT();
    }

  Exit:
    if ( error )
      FT_FREE( idx->offsets );

    return error;
  }

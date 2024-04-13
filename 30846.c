  cff_index_init( CFF_Index  idx,
                  FT_Stream  stream,
                  FT_Bool    load )
  {
    FT_Error   error;
    FT_Memory  memory = stream->memory;
    FT_UShort  count;


    FT_MEM_ZERO( idx, sizeof ( *idx ) );

    idx->stream = stream;
    idx->start  = FT_STREAM_POS();
    if ( !FT_READ_USHORT( count ) &&
         count > 0                )
    {
      FT_Byte   offsize;
      FT_ULong  size;


      /* there is at least one element; read the offset size,           */
      /* then access the offset table to compute the index's total size */
      if ( FT_READ_BYTE( offsize ) )
        goto Exit;

      if ( offsize < 1 || offsize > 4 )
      {
        error = FT_Err_Invalid_Table;
        goto Exit;
      }

      idx->count    = count;
      idx->off_size = offsize;
      size          = (FT_ULong)( count + 1 ) * offsize;

      idx->data_offset = idx->start + 3 + size;

      if ( FT_STREAM_SKIP( size - offsize ) )
        goto Exit;

      size = cff_index_read_offset( idx, &error );
      if ( error )
        goto Exit;

      if ( size == 0 )
      {
        error = CFF_Err_Invalid_Table;
        goto Exit;
      }

      idx->data_size = --size;

      if ( load )
      {
        /* load the data */
        if ( FT_FRAME_EXTRACT( size, idx->bytes ) )
          goto Exit;
      }
      else
      {
        /* skip the data */
        if ( FT_STREAM_SKIP( size ) )
          goto Exit;
      }
    }

  Exit:
    if ( error )
      FT_FREE( idx->offsets );

    return error;
  }

  cff_index_get_pointers( CFF_Index   idx,
                          FT_Byte***  table )
  {
    FT_Error   error  = CFF_Err_Ok;
    FT_Memory  memory = idx->stream->memory;
    FT_ULong   n, offset, old_offset;
    FT_Byte**  t;


    *table = 0;

    if ( idx->offsets == NULL )
    {
      error = cff_index_load_offsets( idx );
      if ( error )
        goto Exit;
    }

    if ( idx->count > 0 && !FT_NEW_ARRAY( t, idx->count + 1 ) )
    {
      old_offset = 1;
      for ( n = 0; n <= idx->count; n++ )
      {
        /* at this point, `idx->offsets' can't be NULL */
        offset = idx->offsets[n];
        if ( !offset )
          offset = old_offset;

        /* two sanity checks for invalid offset tables */
        else if ( offset < old_offset )
          offset = old_offset;

        else if ( offset - 1 >= idx->data_size && n < idx->count )
          offset = old_offset;

        t[n] = idx->bytes + offset - 1;

        old_offset = offset;
      }
      *table = t;
    }

  Exit:
    return error;
  }

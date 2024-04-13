  cff_index_read_offset( CFF_Index  idx,
                         FT_Error  *errorp )
  {
    FT_Error   error;
    FT_Stream  stream = idx->stream;
    FT_Byte    tmp[4];
    FT_ULong   result = 0;


    if ( !FT_STREAM_READ( tmp, idx->off_size ) )
    {
      FT_Int  nn;


      for ( nn = 0; nn < idx->off_size; nn++ )
        result = ( result << 8 ) | tmp[nn];
    }

    *errorp = error;
    return result;
  }

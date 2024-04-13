  FT_Stream_Skip( FT_Stream  stream,
                  FT_Long    distance )
  {
    if ( distance < 0 )
      return FT_Err_Invalid_Stream_Operation;

    return FT_Stream_Seek( stream, (FT_ULong)( stream->pos + distance ) );
  }

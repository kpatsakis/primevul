  FT_Stream_GetOffset( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_Long   result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    if ( p + 2 < stream->limit )
      result       = FT_NEXT_OFF3( p );
    stream->cursor = p;
    return result;
  }

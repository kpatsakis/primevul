  FT_Stream_GetShort( FT_Stream  stream )
  {
    FT_Byte*  p;
    FT_Short  result;


    FT_ASSERT( stream && stream->cursor );

    result         = 0;
    p              = stream->cursor;
    if ( p + 1 < stream->limit )
      result       = FT_NEXT_SHORT( p );
    stream->cursor = p;

    return result;
  }

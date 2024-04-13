  FT_Stream_GetChar( FT_Stream  stream )
  {
    FT_Char  result;


    FT_ASSERT( stream && stream->cursor );

    result = 0;
    if ( stream->cursor < stream->limit )
      result = *stream->cursor++;

    return result;
  }

  FT_Stream_Close( FT_Stream  stream )
  {
    if ( stream && stream->close )
      stream->close( stream );
  }

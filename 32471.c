  FT_Stream_ReadLong( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte   reads[4];
    FT_Byte*  p = 0;
    FT_Long   result = 0;


    FT_ASSERT( stream );

    *error = FT_Err_Ok;

    if ( stream->pos + 3 < stream->size )
    {
      if ( stream->read )
      {
        if ( stream->read( stream, stream->pos, reads, 4L ) != 4L )
          goto Fail;

        p = reads;
      }
      else
      {
        p = stream->base + stream->pos;
      }

      if ( p )
        result = FT_NEXT_LONG( p );
    }
    else
      goto Fail;

    stream->pos += 4;

    return result;

  Fail:
    *error = FT_Err_Invalid_Stream_Operation;
    FT_ERROR(( "FT_Stream_ReadLong:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    return 0;
  }

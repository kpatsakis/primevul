  FT_Stream_ReadShortLE( FT_Stream  stream,
                         FT_Error*  error )
  {
    FT_Byte   reads[2];
    FT_Byte*  p = 0;
    FT_Short  result = 0;


    FT_ASSERT( stream );

    *error = FT_Err_Ok;

    if ( stream->pos + 1 < stream->size )
    {
      if ( stream->read )
      {
        if ( stream->read( stream, stream->pos, reads, 2L ) != 2L )
          goto Fail;

        p = reads;
      }
      else
      {
        p = stream->base + stream->pos;
      }

      if ( p )
        result = FT_NEXT_SHORT_LE( p );
    }
    else
      goto Fail;

    stream->pos += 2;

    return result;

  Fail:
    *error = FT_Err_Invalid_Stream_Operation;
    FT_ERROR(( "FT_Stream_ReadShortLE:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    return 0;
  }

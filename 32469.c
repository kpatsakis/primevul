  FT_Stream_ReadChar( FT_Stream  stream,
                      FT_Error*  error )
  {
    FT_Byte  result = 0;


    FT_ASSERT( stream );

    *error = FT_Err_Ok;

    if ( stream->read )
    {
      if ( stream->read( stream, stream->pos, &result, 1L ) != 1L )
        goto Fail;
    }
    else
    {
      if ( stream->pos < stream->size )
        result = stream->base[stream->pos];
      else
        goto Fail;
    }
    stream->pos++;

    return result;

  Fail:
    *error = FT_Err_Invalid_Stream_Operation;
    FT_ERROR(( "FT_Stream_ReadChar:"
               " invalid i/o; pos = 0x%lx, size = 0x%lx\n",
               stream->pos, stream->size ));

    return 0;
  }

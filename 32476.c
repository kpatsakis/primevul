  FT_Stream_ReleaseFrame( FT_Stream  stream,
                          FT_Byte**  pbytes )
  {
    if ( stream && stream->read )
    {
      FT_Memory  memory = stream->memory;

#ifdef FT_DEBUG_MEMORY
      ft_mem_free( memory, *pbytes );
      *pbytes = NULL;
#else
      FT_FREE( *pbytes );
#endif
    }
    *pbytes = 0;
  }

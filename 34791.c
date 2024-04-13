  check_type1_format( FT_Stream    stream,
                      const char*  header_string,
                      size_t       header_length )
  {
    FT_Error   error;
    FT_UShort  tag;
    FT_ULong   dummy;


    if ( FT_STREAM_SEEK( 0 ) )
      goto Exit;

    error = read_pfb_tag( stream, &tag, &dummy );
    if ( error )
      goto Exit;

    /* We assume that the first segment in a PFB is always encoded as   */
    /* text.  This might be wrong (and the specification doesn't insist */
    /* on that), but we have never seen a counterexample.               */
    if ( tag != 0x8001U && FT_STREAM_SEEK( 0 ) )
      goto Exit;

    if ( !FT_FRAME_ENTER( header_length ) )
    {
      error = FT_Err_Ok;

      if ( ft_memcmp( stream->cursor, header_string, header_length ) != 0 )
        error = FT_THROW( Unknown_File_Format );

      FT_FRAME_EXIT();
    }

  Exit:
    return error;
  }

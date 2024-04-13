  T1_New_Parser( T1_Parser      parser,
                 FT_Stream      stream,
                 FT_Memory      memory,
                 PSAux_Service  psaux )
  {
    FT_Error   error;
    FT_UShort  tag;
    FT_ULong   size;


    psaux->ps_parser_funcs->init( &parser->root, NULL, NULL, memory );

    parser->stream       = stream;
    parser->base_len     = 0;
    parser->base_dict    = NULL;
    parser->private_len  = 0;
    parser->private_dict = NULL;
    parser->in_pfb       = 0;
    parser->in_memory    = 0;
    parser->single_block = 0;

    /* check the header format */
    error = check_type1_format( stream, "%!PS-AdobeFont", 14 );
    if ( error )
    {
      if ( FT_ERR_NEQ( error, Unknown_File_Format ) )
        goto Exit;

      error = check_type1_format( stream, "%!FontType", 10 );
      if ( error )
      {
        FT_TRACE2(( "  not a Type 1 font\n" ));
        goto Exit;
      }
    }

    /******************************************************************/
    /*                                                                */
    /* Here a short summary of what is going on:                      */
    /*                                                                */
    /*   When creating a new Type 1 parser, we try to locate and load */
    /*   the base dictionary if this is possible (i.e., for PFB       */
    /*   files).  Otherwise, we load the whole font into memory.      */
    /*                                                                */
    /*   When `loading' the base dictionary, we only setup pointers   */
    /*   in the case of a memory-based stream.  Otherwise, we         */
    /*   allocate and load the base dictionary in it.                 */
    /*                                                                */
    /*   parser->in_pfb is set if we are in a binary (`.pfb') font.   */
    /*   parser->in_memory is set if we have a memory stream.         */
    /*                                                                */

    /* try to compute the size of the base dictionary;     */
    /* look for a Postscript binary file tag, i.e., 0x8001 */
    if ( FT_STREAM_SEEK( 0L ) )
      goto Exit;

    error = read_pfb_tag( stream, &tag, &size );
    if ( error )
      goto Exit;

    if ( tag != 0x8001U )
    {
      /* assume that this is a PFA file for now; an error will */
      /* be produced later when more things are checked        */
      if ( FT_STREAM_SEEK( 0L ) )
        goto Exit;
      size = stream->size;
    }
    else
      parser->in_pfb = 1;

    /* now, try to load `size' bytes of the `base' dictionary we */
    /* found previously                                          */

    /* if it is a memory-based resource, set up pointers */
    if ( !stream->read )
    {
      parser->base_dict = (FT_Byte*)stream->base + stream->pos;
      parser->base_len  = size;
      parser->in_memory = 1;

      /* check that the `size' field is valid */
      if ( FT_STREAM_SKIP( size ) )
        goto Exit;
    }
    else
    {
      /* read segment in memory -- this is clumsy, but so does the format */
      if ( FT_ALLOC( parser->base_dict, size )       ||
           FT_STREAM_READ( parser->base_dict, size ) )
        goto Exit;
      parser->base_len = size;
    }

    parser->root.base   = parser->base_dict;
    parser->root.cursor = parser->base_dict;
    parser->root.limit  = parser->root.cursor + parser->base_len;

  Exit:
    if ( error && !parser->in_memory )
      FT_FREE( parser->base_dict );

    return error;
  }

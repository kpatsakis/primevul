  ps_parser_to_bytes( PS_Parser  parser,
                      FT_Byte*   bytes,
                      FT_Offset  max_bytes,
                      FT_ULong*  pnum_bytes,
                      FT_Bool    delimiters )
  {
    FT_Error  error = FT_Err_Ok;
    FT_Byte*  cur;


    ps_parser_skip_spaces( parser );
    cur = parser->cursor;

    if ( cur >= parser->limit )
      goto Exit;

    if ( delimiters )
    {
      if ( *cur != '<' )
      {
        FT_ERROR(( "ps_parser_to_bytes: Missing starting delimiter `<'\n" ));
        error = FT_THROW( Invalid_File_Format );
        goto Exit;
      }

      cur++;
    }

    *pnum_bytes = PS_Conv_ASCIIHexDecode( &cur,
                                          parser->limit,
                                          bytes,
                                          max_bytes );

    if ( delimiters )
    {
      if ( cur < parser->limit && *cur != '>' )
      {
        FT_ERROR(( "ps_parser_to_bytes: Missing closing delimiter `>'\n" ));
        error = FT_THROW( Invalid_File_Format );
        goto Exit;
      }

      cur++;
    }

    parser->cursor = cur;

  Exit:
    return error;
  }

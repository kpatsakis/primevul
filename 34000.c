  cff_parse_multiple_master( CFF_Parser  parser )
  {
    CFF_FontRecDict  dict = (CFF_FontRecDict)parser->object;
    FT_Error         error;


#ifdef FT_DEBUG_LEVEL_TRACE
    /* beautify tracing message */
    if ( ft_trace_levels[FT_COMPONENT] < 4 )
      FT_TRACE1(( "Multiple Master CFFs not supported yet,"
                  " handling first master design only\n" ));
    else
      FT_TRACE1(( " (not supported yet,"
                  " handling first master design only)\n" ));
#endif

    error = FT_ERR( Stack_Underflow );

    /* currently, we handle only the first argument */
    if ( parser->top >= parser->stack + 5 )
    {
      FT_Long  num_designs = cff_parse_num( parser, parser->stack );


      if ( num_designs > 16 || num_designs < 2 )
      {
        FT_ERROR(( "cff_parse_multiple_master:"
                   " Invalid number of designs\n" ));
        error = FT_THROW( Invalid_File_Format );
      }
      else
      {
        dict->num_designs   = (FT_UShort)num_designs;
        dict->num_axes      = (FT_UShort)( parser->top - parser->stack - 4 );

        parser->num_designs = dict->num_designs;
        parser->num_axes    = dict->num_axes;

        error = FT_Err_Ok;
      }
    }

    return error;
  }

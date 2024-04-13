  cff_parse_vsindex( CFF_Parser  parser )
  {
    /* vsindex operator can only be used in a Private DICT */
    CFF_Private  priv = (CFF_Private)parser->object;
    FT_Byte**    data = parser->stack;
    CFF_Blend    blend;
    FT_Error     error;


    if ( !priv || !priv->subfont )
    {
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    blend = &priv->subfont->blend;

    if ( blend->usedBV )
    {
      FT_ERROR(( " cff_parse_vsindex: vsindex not allowed after blend\n" ));
      error = FT_THROW( Syntax_Error );
      goto Exit;
    }

    priv->vsindex = (FT_UInt)cff_parse_num( parser, data++ );

    FT_TRACE4(( " %d\n", priv->vsindex ));

    error = FT_Err_Ok;

  Exit:
    return error;
  }

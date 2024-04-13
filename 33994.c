  cff_parse_blend( CFF_Parser  parser )
  {
    /* blend operator can only be used in a Private DICT */
    CFF_Private  priv = (CFF_Private)parser->object;
    CFF_SubFont  subFont;
    CFF_Blend    blend;
    FT_UInt      numBlends;
    FT_Error     error;


    error = FT_ERR( Stack_Underflow );

    if ( !priv || !priv->subfont )
    {
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    subFont = priv->subfont;
    blend   = &subFont->blend;

    if ( cff_blend_check_vector( blend,
                                 priv->vsindex,
                                 subFont->lenNDV,
                                 subFont->NDV ) )
    {
      error = cff_blend_build_vector( blend,
                                      priv->vsindex,
                                      subFont->lenNDV,
                                      subFont->NDV );
      if ( error != FT_Err_Ok )
        goto Exit;
    }

    numBlends = (FT_UInt)cff_parse_num( parser, parser->top - 1 );

    FT_TRACE4(( "   %d values blended\n", numBlends ));

    error = cff_blend_doBlend( subFont, parser, numBlends );

    blend->usedBV = TRUE;

  Exit:
    return error;
  }

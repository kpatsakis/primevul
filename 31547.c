  parse_weight_vector( T1_Face    face,
                       T1_Loader  loader )
  {
    T1_TokenRec  design_tokens[T1_MAX_MM_DESIGNS];
    FT_Int       num_designs;
    FT_Error     error  = FT_Err_Ok;
    T1_Parser    parser = &loader->parser;
    PS_Blend     blend  = face->blend;
    T1_Token     token;
    FT_Int       n;
    FT_Byte*     old_cursor;
    FT_Byte*     old_limit;


    T1_ToTokenArray( parser, design_tokens,
                     T1_MAX_MM_DESIGNS, &num_designs );
    if ( num_designs < 0 )
    {
      error = FT_ERR( Ignore );
      goto Exit;
    }
    if ( num_designs == 0 || num_designs > T1_MAX_MM_DESIGNS )
    {
      FT_ERROR(( "parse_weight_vector:"
                 " incorrect number of designs: %d\n",
                 num_designs ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    if ( !blend || !blend->num_designs )
    {
      error = t1_allocate_blend( face, num_designs, 0 );
      if ( error )
        goto Exit;
      blend = face->blend;
    }
    else if ( blend->num_designs != (FT_UInt)num_designs )
    {
      FT_ERROR(( "parse_weight_vector:"
                 " /BlendDesignPosition and /WeightVector have\n"
                 "                    "
                 " different number of elements\n" ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    old_cursor = parser->root.cursor;
    old_limit  = parser->root.limit;

    for ( n = 0; n < num_designs; n++ )
    {
      token = design_tokens + n;
      parser->root.cursor = token->start;
      parser->root.limit  = token->limit;

      blend->default_weight_vector[n] =
      blend->weight_vector[n]         = T1_ToFixed( parser, 0 );
    }

    parser->root.cursor = old_cursor;
    parser->root.limit  = old_limit;

  Exit:
    parser->root.error = error;
  }

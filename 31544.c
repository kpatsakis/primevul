  parse_blend_design_positions( T1_Face    face,
                                T1_Loader  loader )
  {
    T1_TokenRec  design_tokens[T1_MAX_MM_DESIGNS];
    FT_Int       num_designs;
    FT_Int       num_axis;
    T1_Parser    parser = &loader->parser;

    FT_Error     error = FT_Err_Ok;
    PS_Blend     blend;


    /* get the array of design tokens -- compute number of designs */
    T1_ToTokenArray( parser, design_tokens,
                     T1_MAX_MM_DESIGNS, &num_designs );
    if ( num_designs < 0 )
    {
      error = FT_ERR( Ignore );
      goto Exit;
    }
    if ( num_designs == 0 || num_designs > T1_MAX_MM_DESIGNS )
    {
      FT_ERROR(( "parse_blend_design_positions:"
                 " incorrect number of designs: %d\n",
                 num_designs ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    {
      FT_Byte*  old_cursor = parser->root.cursor;
      FT_Byte*  old_limit  = parser->root.limit;
      FT_Int    n;


      blend    = face->blend;
      num_axis = 0;  /* make compiler happy */

      for ( n = 0; n < num_designs; n++ )
      {
        T1_TokenRec  axis_tokens[T1_MAX_MM_AXIS];
        T1_Token     token;
        FT_Int       axis, n_axis;


        /* read axis/coordinates tokens */
        token = design_tokens + n;
        parser->root.cursor = token->start;
        parser->root.limit  = token->limit;
        T1_ToTokenArray( parser, axis_tokens, T1_MAX_MM_AXIS, &n_axis );

        if ( n == 0 )
        {
          if ( n_axis <= 0 || n_axis > T1_MAX_MM_AXIS )
          {
            FT_ERROR(( "parse_blend_design_positions:"
                       " invalid number of axes: %d\n",
                       n_axis ));
            error = FT_THROW( Invalid_File_Format );
            goto Exit;
          }

          num_axis = n_axis;
          error = t1_allocate_blend( face, num_designs, num_axis );
          if ( error )
            goto Exit;
          blend = face->blend;
        }
        else if ( n_axis != num_axis )
        {
          FT_ERROR(( "parse_blend_design_positions: incorrect table\n" ));
          error = FT_THROW( Invalid_File_Format );
          goto Exit;
        }

        /* now read each axis token into the design position */
        for ( axis = 0; axis < n_axis; axis++ )
        {
          T1_Token  token2 = axis_tokens + axis;


          parser->root.cursor = token2->start;
          parser->root.limit  = token2->limit;
          blend->design_pos[n][axis] = T1_ToFixed( parser, 0 );
        }
      }

      loader->parser.root.cursor = old_cursor;
      loader->parser.root.limit  = old_limit;
    }

  Exit:
    loader->parser.root.error = error;
  }

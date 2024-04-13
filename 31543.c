  parse_blend_design_map( T1_Face    face,
                          T1_Loader  loader )
  {
    FT_Error     error  = FT_Err_Ok;
    T1_Parser    parser = &loader->parser;
    PS_Blend     blend;
    T1_TokenRec  axis_tokens[T1_MAX_MM_AXIS];
    FT_Int       n, num_axis;
    FT_Byte*     old_cursor;
    FT_Byte*     old_limit;
    FT_Memory    memory = face->root.memory;


    T1_ToTokenArray( parser, axis_tokens,
                     T1_MAX_MM_AXIS, &num_axis );
    if ( num_axis < 0 )
    {
      error = FT_ERR( Ignore );
      goto Exit;
    }
    if ( num_axis == 0 || num_axis > T1_MAX_MM_AXIS )
    {
      FT_ERROR(( "parse_blend_design_map: incorrect number of axes: %d\n",
                 num_axis ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    old_cursor = parser->root.cursor;
    old_limit  = parser->root.limit;

    error = t1_allocate_blend( face, 0, num_axis );
    if ( error )
      goto Exit;
    blend = face->blend;

    /* now read each axis design map */
    for ( n = 0; n < num_axis; n++ )
    {
      PS_DesignMap  map = blend->design_map + n;
      T1_Token      axis_token;
      T1_TokenRec   point_tokens[T1_MAX_MM_MAP_POINTS];
      FT_Int        p, num_points;


      axis_token = axis_tokens + n;

      parser->root.cursor = axis_token->start;
      parser->root.limit  = axis_token->limit;
      T1_ToTokenArray( parser, point_tokens,
                       T1_MAX_MM_MAP_POINTS, &num_points );

      if ( num_points <= 0 || num_points > T1_MAX_MM_MAP_POINTS )
      {
        FT_ERROR(( "parse_blend_design_map: incorrect table\n" ));
        error = FT_THROW( Invalid_File_Format );
        goto Exit;
      }

      /* allocate design map data */
      if ( FT_NEW_ARRAY( map->design_points, num_points * 2 ) )
        goto Exit;
      map->blend_points = map->design_points + num_points;
      map->num_points   = (FT_Byte)num_points;

      for ( p = 0; p < num_points; p++ )
      {
        T1_Token  point_token;


        point_token = point_tokens + p;

        /* don't include delimiting brackets */
        parser->root.cursor = point_token->start + 1;
        parser->root.limit  = point_token->limit - 1;

        map->design_points[p] = T1_ToInt( parser );
        map->blend_points [p] = T1_ToFixed( parser, 0 );
      }
    }

    parser->root.cursor = old_cursor;
    parser->root.limit  = old_limit;

  Exit:
    parser->root.error = error;
  }

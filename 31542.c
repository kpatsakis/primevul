  parse_blend_axis_types( T1_Face    face,
                          T1_Loader  loader )
  {
    T1_TokenRec  axis_tokens[T1_MAX_MM_AXIS];
    FT_Int       n, num_axis;
    FT_Error     error = FT_Err_Ok;
    PS_Blend     blend;
    FT_Memory    memory;


    /* take an array of objects */
    T1_ToTokenArray( &loader->parser, axis_tokens,
                     T1_MAX_MM_AXIS, &num_axis );
    if ( num_axis < 0 )
    {
      error = FT_ERR( Ignore );
      goto Exit;
    }
    if ( num_axis == 0 || num_axis > T1_MAX_MM_AXIS )
    {
      FT_ERROR(( "parse_blend_axis_types: incorrect number of axes: %d\n",
                 num_axis ));
      error = FT_THROW( Invalid_File_Format );
      goto Exit;
    }

    /* allocate blend if necessary */
    error = t1_allocate_blend( face, 0, (FT_UInt)num_axis );
    if ( error )
      goto Exit;

    blend  = face->blend;
    memory = face->root.memory;

    /* each token is an immediate containing the name of the axis */
    for ( n = 0; n < num_axis; n++ )
    {
      T1_Token    token = axis_tokens + n;
      FT_Byte*    name;
      FT_PtrDist  len;


      /* skip first slash, if any */
      if ( token->start[0] == '/' )
        token->start++;

      len = token->limit - token->start;
      if ( len == 0 )
      {
        error = FT_THROW( Invalid_File_Format );
        goto Exit;
      }

      if ( FT_ALLOC( blend->axis_names[n], (FT_Long)( len + 1 ) ) )
        goto Exit;

      name = (FT_Byte*)blend->axis_names[n];
      FT_MEM_COPY( name, token->start, len );
      name[len] = '\0';
    }

  Exit:
    loader->parser.root.error = error;
  }

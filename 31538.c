  T1_Get_MM_Var( T1_Face      face,
                 FT_MM_Var*  *master )
  {
    FT_Memory        memory = face->root.memory;
    FT_MM_Var       *mmvar = NULL;
    FT_Multi_Master  mmaster;
    FT_Error         error;
    FT_UInt          i;
    FT_Fixed         axiscoords[T1_MAX_MM_AXIS];
    PS_Blend         blend = face->blend;


    error = T1_Get_Multi_Master( face, &mmaster );
    if ( error )
      goto Exit;
    if ( FT_ALLOC( mmvar,
                   sizeof ( FT_MM_Var ) +
                     mmaster.num_axis * sizeof ( FT_Var_Axis ) ) )
      goto Exit;

    mmvar->num_axis        = mmaster.num_axis;
    mmvar->num_designs     = mmaster.num_designs;
    mmvar->num_namedstyles = ~0U;                        /* Does not apply */
    mmvar->axis            = (FT_Var_Axis*)&mmvar[1];
                                      /* Point to axes after MM_Var struct */
    mmvar->namedstyle      = NULL;

    for ( i = 0 ; i < mmaster.num_axis; ++i )
    {
      mmvar->axis[i].name    = mmaster.axis[i].name;
      mmvar->axis[i].minimum = INT_TO_FIXED( mmaster.axis[i].minimum);
      mmvar->axis[i].maximum = INT_TO_FIXED( mmaster.axis[i].maximum);
      mmvar->axis[i].def     = ( mmvar->axis[i].minimum +
                                   mmvar->axis[i].maximum ) / 2;
                            /* Does not apply.  But this value is in range */
      mmvar->axis[i].strid   = ~0U;                      /* Does not apply */
      mmvar->axis[i].tag     = ~0U;                      /* Does not apply */

      if ( ft_strcmp( mmvar->axis[i].name, "Weight" ) == 0 )
        mmvar->axis[i].tag = FT_MAKE_TAG( 'w', 'g', 'h', 't' );
      else if ( ft_strcmp( mmvar->axis[i].name, "Width" ) == 0 )
        mmvar->axis[i].tag = FT_MAKE_TAG( 'w', 'd', 't', 'h' );
      else if ( ft_strcmp( mmvar->axis[i].name, "OpticalSize" ) == 0 )
        mmvar->axis[i].tag = FT_MAKE_TAG( 'o', 'p', 's', 'z' );
    }

    if ( blend->num_designs == ( 1U << blend->num_axis ) )
    {
      mm_weights_unmap( blend->default_weight_vector,
                        axiscoords,
                        blend->num_axis );

      for ( i = 0; i < mmaster.num_axis; ++i )
        mmvar->axis[i].def = mm_axis_unmap( &blend->design_map[i],
                                            axiscoords[i] );
    }

    *master = mmvar;

  Exit:
    return error;
  }

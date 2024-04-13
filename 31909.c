  TT_Set_Var_Design( TT_Face    face,
                     FT_UInt    num_coords,
                     FT_Fixed*  coords )
  {
    FT_Error        error      = TT_Err_Ok;
    FT_Fixed*       normalized = NULL;
    GX_Blend        blend;
    FT_MM_Var*      mmvar;
    FT_UInt         i, j;
    FT_Var_Axis*    a;
    GX_AVarSegment  av;
    FT_Memory       memory = face->root.memory;


    if ( face->blend == NULL )
    {
      if ( (error = TT_Get_MM_Var( face, NULL )) != 0 )
        goto Exit;
    }

    blend = face->blend;
    mmvar = blend->mmvar;

    if ( num_coords != mmvar->num_axis )
    {
      error = TT_Err_Invalid_Argument;
      goto Exit;
    }

    /* Axis normalization is a two stage process.  First we normalize */
    /* based on the [min,def,max] values for the axis to be [-1,0,1]. */
    /* Then, if there's an `avar' table, we renormalize this range.   */

    if ( FT_NEW_ARRAY( normalized, mmvar->num_axis ) )
      goto Exit;

    a = mmvar->axis;
    for ( i = 0; i < mmvar->num_axis; ++i, ++a )
    {
      if ( coords[i] > a->maximum || coords[i] < a->minimum )
      {
        error = TT_Err_Invalid_Argument;
        goto Exit;
      }

      if ( coords[i] < a->def )
      {
        normalized[i] = -FT_MulDiv( coords[i] - a->def,
                                    0x10000L,
                                    a->minimum - a->def );
      }
      else if ( a->maximum == a->def )
        normalized[i] = 0;
      else
      {
        normalized[i] = FT_MulDiv( coords[i] - a->def,
                                   0x10000L,
                                   a->maximum - a->def );
      }
    }

    if ( !blend->avar_checked )
      ft_var_load_avar( face );

    if ( blend->avar_segment != NULL )
    {
      av = blend->avar_segment;
      for ( i = 0; i < mmvar->num_axis; ++i, ++av )
      {
        for ( j = 1; j < (FT_UInt)av->pairCount; ++j )
          if ( normalized[i] < av->correspondence[j].fromCoord )
          {
            normalized[i] =
              FT_MulDiv(
                FT_MulDiv(
                  normalized[i] - av->correspondence[j - 1].fromCoord,
                  0x10000L,
                  av->correspondence[j].fromCoord -
                    av->correspondence[j - 1].fromCoord ),
                av->correspondence[j].toCoord -
                  av->correspondence[j - 1].toCoord,
                0x10000L ) +
              av->correspondence[j - 1].toCoord;
            break;
          }
      }
    }

    error = TT_Set_MM_Blend( face, num_coords, normalized );

  Exit:
    FT_FREE( normalized );
    return error;
  }

  TT_Get_MM_Var( TT_Face      face,
                 FT_MM_Var*  *master )
  {
    FT_Stream            stream = face->root.stream;
    FT_Memory            memory = face->root.memory;
    FT_ULong             table_len;
    FT_Error             error  = TT_Err_Ok;
    FT_ULong             fvar_start;
    FT_Int               i, j;
    FT_MM_Var*           mmvar;
    FT_Fixed*            next_coords;
    FT_String*           next_name;
    FT_Var_Axis*         a;
    FT_Var_Named_Style*  ns;
    GX_FVar_Head         fvar_head;

    static const FT_Frame_Field  fvar_fields[] =
    {

#undef  FT_STRUCTURE
#define FT_STRUCTURE  GX_FVar_Head

      FT_FRAME_START( 16 ),
        FT_FRAME_LONG  ( version ),
        FT_FRAME_USHORT( offsetToData ),
        FT_FRAME_USHORT( countSizePairs ),
        FT_FRAME_USHORT( axisCount ),
        FT_FRAME_USHORT( axisSize ),
        FT_FRAME_USHORT( instanceCount ),
        FT_FRAME_USHORT( instanceSize ),
      FT_FRAME_END
    };

    static const FT_Frame_Field  fvaraxis_fields[] =
    {

#undef  FT_STRUCTURE
#define FT_STRUCTURE  GX_FVar_Axis

      FT_FRAME_START( 20 ),
        FT_FRAME_ULONG ( axisTag ),
        FT_FRAME_ULONG ( minValue ),
        FT_FRAME_ULONG ( defaultValue ),
        FT_FRAME_ULONG ( maxValue ),
        FT_FRAME_USHORT( flags ),
        FT_FRAME_USHORT( nameID ),
      FT_FRAME_END
    };


    if ( face->blend == NULL )
    {
      /* both `fvar' and `gvar' must be present */
      if ( (error = face->goto_table( face, TTAG_gvar,
                                      stream, &table_len )) != 0 )
        goto Exit;

      if ( (error = face->goto_table( face, TTAG_fvar,
                                      stream, &table_len )) != 0 )
        goto Exit;

      fvar_start = FT_STREAM_POS( );

      if ( FT_STREAM_READ_FIELDS( fvar_fields, &fvar_head ) )
        goto Exit;

      if ( fvar_head.version != (FT_Long)0x00010000L                      ||
           fvar_head.countSizePairs != 2                                  ||
           fvar_head.axisSize != 20                                       ||
           /* axisCount limit implied by 16-bit instanceSize */
           fvar_head.axisCount > 0x3FFE                                   ||
           fvar_head.instanceSize != 4 + 4 * fvar_head.axisCount          ||
           /* instanceCount limit implied by limited range of name IDs */
           fvar_head.instanceCount > 0x7EFF                               ||
           fvar_head.offsetToData + fvar_head.axisCount * 20U +
             fvar_head.instanceCount * fvar_head.instanceSize > table_len )
      {
        error = TT_Err_Invalid_Table;
        goto Exit;
      }

      if ( FT_NEW( face->blend ) )
        goto Exit;

      /* cannot overflow 32-bit arithmetic because of limits above */
      face->blend->mmvar_len =
        sizeof ( FT_MM_Var ) +
        fvar_head.axisCount * sizeof ( FT_Var_Axis ) +
        fvar_head.instanceCount * sizeof ( FT_Var_Named_Style ) +
        fvar_head.instanceCount * fvar_head.axisCount * sizeof ( FT_Fixed ) +
        5 * fvar_head.axisCount;

      if ( FT_ALLOC( mmvar, face->blend->mmvar_len ) )
        goto Exit;
      face->blend->mmvar = mmvar;

      mmvar->num_axis =
        fvar_head.axisCount;
      mmvar->num_designs =
        (FT_UInt)-1;           /* meaningless in this context; each glyph */
                               /* may have a different number of designs  */
                               /* (or tuples, as called by Apple)         */
      mmvar->num_namedstyles =
        fvar_head.instanceCount;
      mmvar->axis =
        (FT_Var_Axis*)&(mmvar[1]);
      mmvar->namedstyle =
        (FT_Var_Named_Style*)&(mmvar->axis[fvar_head.axisCount]);

      next_coords =
        (FT_Fixed*)&(mmvar->namedstyle[fvar_head.instanceCount]);
      for ( i = 0; i < fvar_head.instanceCount; ++i )
      {
        mmvar->namedstyle[i].coords  = next_coords;
        next_coords                 += fvar_head.axisCount;
      }

      next_name = (FT_String*)next_coords;
      for ( i = 0; i < fvar_head.axisCount; ++i )
      {
        mmvar->axis[i].name  = next_name;
        next_name           += 5;
      }

      if ( FT_STREAM_SEEK( fvar_start + fvar_head.offsetToData ) )
        goto Exit;

      a = mmvar->axis;
      for ( i = 0; i < fvar_head.axisCount; ++i )
      {
        GX_FVar_Axis  axis_rec;


        if ( FT_STREAM_READ_FIELDS( fvaraxis_fields, &axis_rec ) )
          goto Exit;
        a->tag     = axis_rec.axisTag;
        a->minimum = axis_rec.minValue;     /* A Fixed */
        a->def     = axis_rec.defaultValue; /* A Fixed */
        a->maximum = axis_rec.maxValue;     /* A Fixed */
        a->strid   = axis_rec.nameID;

        a->name[0] = (FT_String)(   a->tag >> 24 );
        a->name[1] = (FT_String)( ( a->tag >> 16 ) & 0xFF );
        a->name[2] = (FT_String)( ( a->tag >>  8 ) & 0xFF );
        a->name[3] = (FT_String)( ( a->tag       ) & 0xFF );
        a->name[4] = 0;

        ++a;
      }

      ns = mmvar->namedstyle;
      for ( i = 0; i < fvar_head.instanceCount; ++i, ++ns )
      {
        if ( FT_FRAME_ENTER( 4L + 4L * fvar_head.axisCount ) )
          goto Exit;

        ns->strid       =    FT_GET_USHORT();
        (void) /* flags = */ FT_GET_USHORT();

        for ( j = 0; j < fvar_head.axisCount; ++j )
          ns->coords[j] = FT_GET_ULONG();     /* A Fixed */

        FT_FRAME_EXIT();
      }
    }

    if ( master != NULL )
    {
      FT_UInt  n;


      if ( FT_ALLOC( mmvar, face->blend->mmvar_len ) )
        goto Exit;
      FT_MEM_COPY( mmvar, face->blend->mmvar, face->blend->mmvar_len );

      mmvar->axis =
        (FT_Var_Axis*)&(mmvar[1]);
      mmvar->namedstyle =
        (FT_Var_Named_Style*)&(mmvar->axis[mmvar->num_axis]);
      next_coords =
        (FT_Fixed*)&(mmvar->namedstyle[mmvar->num_namedstyles]);

      for ( n = 0; n < mmvar->num_namedstyles; ++n )
      {
        mmvar->namedstyle[n].coords  = next_coords;
        next_coords                 += mmvar->num_axis;
      }

      a = mmvar->axis;
      next_name = (FT_String*)next_coords;
      for ( n = 0; n < mmvar->num_axis; ++n )
      {
        a->name = next_name;

        /* standard PostScript names for some standard apple tags */
        if ( a->tag == TTAG_wght )
          a->name = (char *)"Weight";
        else if ( a->tag == TTAG_wdth )
          a->name = (char *)"Width";
        else if ( a->tag == TTAG_opsz )
          a->name = (char *)"OpticalSize";
        else if ( a->tag == TTAG_slnt )
          a->name = (char *)"Slant";

        next_name += 5;
        ++a;
      }

      *master = mmvar;
    }

  Exit:
    return error;
  }

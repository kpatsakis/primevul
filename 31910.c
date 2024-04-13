  TT_Vary_Get_Glyph_Deltas( TT_Face      face,
                            FT_UInt      glyph_index,
                            FT_Vector*  *deltas,
                            FT_UInt      n_points )
  {
    FT_Stream   stream = face->root.stream;
    FT_Memory   memory = stream->memory;
    GX_Blend    blend  = face->blend;
    FT_Vector*  delta_xy;

    FT_Error    error;
    FT_ULong    glyph_start;
    FT_UInt     tupleCount;
    FT_ULong    offsetToData;
    FT_ULong    here;
    FT_UInt     i, j;
    FT_Fixed*   tuple_coords    = NULL;
    FT_Fixed*   im_start_coords = NULL;
    FT_Fixed*   im_end_coords   = NULL;
    FT_UInt     point_count, spoint_count = 0;
    FT_UShort*  sharedpoints = NULL;
    FT_UShort*  localpoints  = NULL;
    FT_UShort*  points;
    FT_Short    *deltas_x, *deltas_y;


    if ( !face->doblend || blend == NULL )
      return TT_Err_Invalid_Argument;

    /* to be freed by the caller */
    if ( FT_NEW_ARRAY( delta_xy, n_points ) )
      goto Exit;
    *deltas = delta_xy;

    if ( glyph_index >= blend->gv_glyphcnt      ||
         blend->glyphoffsets[glyph_index] ==
           blend->glyphoffsets[glyph_index + 1] )
      return TT_Err_Ok;               /* no variation data for this glyph */

    if ( FT_STREAM_SEEK( blend->glyphoffsets[glyph_index] )   ||
         FT_FRAME_ENTER( blend->glyphoffsets[glyph_index + 1] -
                           blend->glyphoffsets[glyph_index] ) )
      goto Fail1;

    glyph_start = FT_Stream_FTell( stream );

    /* each set of glyph variation data is formatted similarly to `cvar' */
    /* (except we get shared points and global tuples)                   */

    if ( FT_NEW_ARRAY( tuple_coords, blend->num_axis )    ||
         FT_NEW_ARRAY( im_start_coords, blend->num_axis ) ||
         FT_NEW_ARRAY( im_end_coords, blend->num_axis )   )
      goto Fail2;

    tupleCount   = FT_GET_USHORT();
    offsetToData = glyph_start + FT_GET_USHORT();

    if ( tupleCount & GX_TC_TUPLES_SHARE_POINT_NUMBERS )
    {
      here = FT_Stream_FTell( stream );

      FT_Stream_SeekSet( stream, offsetToData );

      sharedpoints = ft_var_readpackedpoints( stream, &spoint_count );
      offsetToData = FT_Stream_FTell( stream );

      FT_Stream_SeekSet( stream, here );
    }

    for ( i = 0; i < ( tupleCount & GX_TC_TUPLE_COUNT_MASK ); ++i )
    {
      FT_UInt   tupleDataSize;
      FT_UInt   tupleIndex;
      FT_Fixed  apply;


      tupleDataSize = FT_GET_USHORT();
      tupleIndex    = FT_GET_USHORT();

      if ( tupleIndex & GX_TI_EMBEDDED_TUPLE_COORD )
      {
        for ( j = 0; j < blend->num_axis; ++j )
          tuple_coords[j] = FT_GET_SHORT() << 2;  /* convert from        */
                                                  /* short frac to fixed */
      }
      else if ( ( tupleIndex & GX_TI_TUPLE_INDEX_MASK ) >= blend->tuplecount )
      {
        error = TT_Err_Invalid_Table;
        goto Fail3;
      }
      else
      {
        FT_MEM_COPY(
          tuple_coords,
          &blend->tuplecoords[(tupleIndex & 0xFFF) * blend->num_axis],
          blend->num_axis * sizeof ( FT_Fixed ) );
      }

      if ( tupleIndex & GX_TI_INTERMEDIATE_TUPLE )
      {
        for ( j = 0; j < blend->num_axis; ++j )
          im_start_coords[j] = FT_GET_SHORT() << 2;
        for ( j = 0; j < blend->num_axis; ++j )
          im_end_coords[j] = FT_GET_SHORT() << 2;
      }

      apply = ft_var_apply_tuple( blend,
                                  (FT_UShort)tupleIndex,
                                  tuple_coords,
                                  im_start_coords,
                                  im_end_coords );

      if ( apply == 0 )              /* tuple isn't active for our blend */
      {
        offsetToData += tupleDataSize;
        continue;
      }

      here = FT_Stream_FTell( stream );

      if ( tupleIndex & GX_TI_PRIVATE_POINT_NUMBERS )
      {
        FT_Stream_SeekSet( stream, offsetToData );

        localpoints = ft_var_readpackedpoints( stream, &point_count );
        points      = localpoints;
      }
      else
      {
        points      = sharedpoints;
        point_count = spoint_count;
      }

      deltas_x = ft_var_readpackeddeltas( stream,
                                          point_count == 0 ? n_points
                                                           : point_count );
      deltas_y = ft_var_readpackeddeltas( stream,
                                          point_count == 0 ? n_points
                                                           : point_count );

      if ( points == NULL || deltas_y == NULL || deltas_x == NULL )
        ; /* failure, ignore it */

      else if ( points == ALL_POINTS )
      {
        /* this means that there are deltas for every point in the glyph */
        for ( j = 0; j < n_points; ++j )
        {
          delta_xy[j].x += FT_MulFix( deltas_x[j], apply );
          delta_xy[j].y += FT_MulFix( deltas_y[j], apply );
        }
      }

      else
      {
        for ( j = 0; j < point_count; ++j )
        {
          delta_xy[localpoints[j]].x += FT_MulFix( deltas_x[j], apply );
          delta_xy[localpoints[j]].y += FT_MulFix( deltas_y[j], apply );
        }
      }

      if ( localpoints != ALL_POINTS )
        FT_FREE( localpoints );
      FT_FREE( deltas_x );
      FT_FREE( deltas_y );

      offsetToData += tupleDataSize;

      FT_Stream_SeekSet( stream, here );
    }

  Fail3:
    FT_FREE( tuple_coords );
    FT_FREE( im_start_coords );
    FT_FREE( im_end_coords );

  Fail2:
    FT_FRAME_EXIT();

  Fail1:
    if ( error )
    {
      FT_FREE( delta_xy );
      *deltas = NULL;
    }

  Exit:
    return error;
  }

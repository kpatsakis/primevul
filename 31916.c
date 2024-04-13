  tt_face_vary_cvt( TT_Face    face,
                    FT_Stream  stream )
  {
    FT_Error    error;
    FT_Memory   memory = stream->memory;
    FT_ULong    table_start;
    FT_ULong    table_len;
    FT_UInt     tupleCount;
    FT_ULong    offsetToData;
    FT_ULong    here;
    FT_UInt     i, j;
    FT_Fixed*   tuple_coords    = NULL;
    FT_Fixed*   im_start_coords = NULL;
    FT_Fixed*   im_end_coords   = NULL;
    GX_Blend    blend           = face->blend;
    FT_UInt     point_count;
    FT_UShort*  localpoints;
    FT_Short*   deltas;


    FT_TRACE2(( "CVAR " ));

    if ( blend == NULL )
    {
      FT_TRACE2(( "tt_face_vary_cvt: no blend specified\n" ));

      error = TT_Err_Ok;
      goto Exit;
    }

    if ( face->cvt == NULL )
    {
      FT_TRACE2(( "tt_face_vary_cvt: no `cvt ' table\n" ));

      error = TT_Err_Ok;
      goto Exit;
    }

    error = face->goto_table( face, TTAG_cvar, stream, &table_len );
    if ( error )
    {
      FT_TRACE2(( "is missing\n" ));

      error = TT_Err_Ok;
      goto Exit;
    }

    if ( FT_FRAME_ENTER( table_len ) )
    {
      error = TT_Err_Ok;
      goto Exit;
    }

    table_start = FT_Stream_FTell( stream );
    if ( FT_GET_LONG() != 0x00010000L )
    {
      FT_TRACE2(( "bad table version\n" ));

      error = TT_Err_Ok;
      goto FExit;
    }

    if ( FT_NEW_ARRAY( tuple_coords, blend->num_axis )    ||
         FT_NEW_ARRAY( im_start_coords, blend->num_axis ) ||
         FT_NEW_ARRAY( im_end_coords, blend->num_axis )   )
      goto FExit;

    tupleCount   = FT_GET_USHORT();
    offsetToData = table_start + FT_GET_USHORT();

    /* The documentation implies there are flags packed into the        */
    /* tuplecount, but John Jenkins says that shared points don't apply */
    /* to `cvar', and no other flags are defined.                       */

    for ( i = 0; i < ( tupleCount & 0xFFF ); ++i )
    {
      FT_UInt   tupleDataSize;
      FT_UInt   tupleIndex;
      FT_Fixed  apply;


      tupleDataSize = FT_GET_USHORT();
      tupleIndex    = FT_GET_USHORT();

      /* There is no provision here for a global tuple coordinate section, */
      /* so John says.  There are no tuple indices, just embedded tuples.  */

      if ( tupleIndex & GX_TI_EMBEDDED_TUPLE_COORD )
      {
        for ( j = 0; j < blend->num_axis; ++j )
          tuple_coords[j] = FT_GET_SHORT() << 2; /* convert from        */
                                                 /* short frac to fixed */
      }
      else
      {
        /* skip this tuple; it makes no sense */

        if ( tupleIndex & GX_TI_INTERMEDIATE_TUPLE )
          for ( j = 0; j < 2 * blend->num_axis; ++j )
            (void)FT_GET_SHORT();

        offsetToData += tupleDataSize;
        continue;
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
      if ( /* tuple isn't active for our blend */
           apply == 0                                    ||
           /* global points not allowed,           */
           /* if they aren't local, makes no sense */
           !( tupleIndex & GX_TI_PRIVATE_POINT_NUMBERS ) )
      {
        offsetToData += tupleDataSize;
        continue;
      }

      here = FT_Stream_FTell( stream );

      FT_Stream_SeekSet( stream, offsetToData );

      localpoints = ft_var_readpackedpoints( stream, &point_count );
      deltas      = ft_var_readpackeddeltas( stream,
                                             point_count == 0 ? face->cvt_size
                                                              : point_count );
      if ( localpoints == NULL || deltas == NULL )
        /* failure, ignore it */;

      else if ( localpoints == ALL_POINTS )
      {
        /* this means that there are deltas for every entry in cvt */
        for ( j = 0; j < face->cvt_size; ++j )
          face->cvt[j] = (FT_Short)( face->cvt[j] +
                                     FT_MulFix( deltas[j], apply ) );
      }

      else
      {
        for ( j = 0; j < point_count; ++j )
        {
          int  pindex = localpoints[j];

          face->cvt[pindex] = (FT_Short)( face->cvt[pindex] +
                                          FT_MulFix( deltas[j], apply ) );
        }
      }

      if ( localpoints != ALL_POINTS )
        FT_FREE( localpoints );
      FT_FREE( deltas );

      offsetToData += tupleDataSize;

      FT_Stream_SeekSet( stream, here );
    }

  FExit:
    FT_FRAME_EXIT();

  Exit:
    FT_FREE( tuple_coords );
    FT_FREE( im_start_coords );
    FT_FREE( im_end_coords );

    return error;
  }

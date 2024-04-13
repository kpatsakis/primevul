  ft_var_load_avar( TT_Face  face )
  {
    FT_Stream       stream = FT_FACE_STREAM(face);
    FT_Memory       memory = stream->memory;
    GX_Blend        blend  = face->blend;
    GX_AVarSegment  segment;
    FT_Error        error = TT_Err_Ok;
    FT_ULong        version;
    FT_Long         axisCount;
    FT_Int          i, j;
    FT_ULong        table_len;

    FT_UNUSED( error );


    blend->avar_checked = TRUE;
    if ( (error = face->goto_table( face, TTAG_avar, stream, &table_len )) != 0 )
      return;

    if ( FT_FRAME_ENTER( table_len ) )
      return;

    version   = FT_GET_LONG();
    axisCount = FT_GET_LONG();

    if ( version != 0x00010000L                       ||
         axisCount != (FT_Long)blend->mmvar->num_axis )
      goto Exit;

    if ( FT_NEW_ARRAY( blend->avar_segment, axisCount ) )
      goto Exit;

    segment = &blend->avar_segment[0];
    for ( i = 0; i < axisCount; ++i, ++segment )
    {
      segment->pairCount = FT_GET_USHORT();
      if ( FT_NEW_ARRAY( segment->correspondence, segment->pairCount ) )
      {
        /* Failure.  Free everything we have done so far.  We must do */
        /* it right now since loading the `avar' table is optional.   */

        for ( j = i - 1; j >= 0; --j )
          FT_FREE( blend->avar_segment[j].correspondence );

        FT_FREE( blend->avar_segment );
        blend->avar_segment = NULL;
        goto Exit;
      }

      for ( j = 0; j < segment->pairCount; ++j )
      {
        segment->correspondence[j].fromCoord =
          FT_GET_SHORT() << 2;    /* convert to Fixed */
        segment->correspondence[j].toCoord =
          FT_GET_SHORT()<<2;    /* convert to Fixed */
      }
    }

  Exit:
    FT_FRAME_EXIT();
  }

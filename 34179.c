static HB_Error  Load_LigatureAttach( HB_LigatureAttach*  lat,
				      HB_UShort            num_classes,
				      HB_Stream            stream )
{
  HB_Error  error;

  HB_UShort             m, n, k, count;
  HB_UInt              cur_offset, new_offset, base_offset;

  HB_ComponentRecord*  cr;
  HB_Anchor*           lan;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = lat->ComponentCount = GET_UShort();

  FORGET_Frame();

  lat->ComponentRecord = NULL;

  if ( ALLOC_ARRAY( lat->ComponentRecord, count, HB_ComponentRecord ) )
    return error;

  cr = lat->ComponentRecord;

  for ( m = 0; m < count; m++ )
  {
    cr[m].LigatureAnchor = NULL;

    if ( ALLOC_ARRAY( cr[m].LigatureAnchor, num_classes, HB_Anchor ) )
      goto Fail;

    lan = cr[m].LigatureAnchor;

    for ( n = 0; n < num_classes; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
	goto Fail0;

      new_offset = GET_UShort();

      FORGET_Frame();

      if ( new_offset )
      {
	new_offset += base_offset;

	cur_offset = FILE_Pos();
	if ( FILE_Seek( new_offset ) ||
	     ( error = Load_Anchor( &lan[n], stream ) ) != HB_Err_Ok )
	  goto Fail0;
	(void)FILE_Seek( cur_offset );
      }
      else
	lan[n].PosFormat = 0;
    }

    continue;
  Fail0:
    for ( k = 0; k < n; k++ )
      Free_Anchor( &lan[k] );
    goto Fail;
  }

  return HB_Err_Ok;

Fail:
  for ( k = 0; k < m; k++ )
  {
    lan = cr[k].LigatureAnchor;

    for ( n = 0; n < num_classes; n++ )
      Free_Anchor( &lan[n] );

    FREE( lan );
  }

  FREE( cr );
  return error;
}

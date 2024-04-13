static HB_Error  Load_LigatureArray( HB_LigatureArray*  la,
				     HB_UShort           num_classes,
				     HB_Stream           stream )
{
  HB_Error  error;

  HB_UShort            n, m, count;
  HB_UInt             cur_offset, new_offset, base_offset;

  HB_LigatureAttach*  lat;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = la->LigatureCount = GET_UShort();

  FORGET_Frame();

  la->LigatureAttach = NULL;

  if ( ALLOC_ARRAY( la->LigatureAttach, count, HB_LigatureAttach ) )
    return error;

  lat = la->LigatureAttach;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_LigatureAttach( &lat[n], num_classes,
					stream ) ) != HB_Err_Ok )
      goto Fail;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
    Free_LigatureAttach( &lat[m], num_classes );

  FREE( lat );
  return error;
}

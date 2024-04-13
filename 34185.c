static HB_Error  Load_PairPos1( HB_PairPosFormat1*  ppf1,
				HB_UShort            format1,
				HB_UShort            format2,
				HB_Stream            stream )
{
  HB_Error  error;

  HB_UShort     n, m, count;
  HB_UInt      cur_offset, new_offset, base_offset;

  HB_PairSet*  ps;


  base_offset = FILE_Pos() - 8L;

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = ppf1->PairSetCount = GET_UShort();

  FORGET_Frame();

  ppf1->PairSet = NULL;

  if ( ALLOC_ARRAY( ppf1->PairSet, count, HB_PairSet ) )
    return error;

  ps = ppf1->PairSet;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_PairSet( &ps[n], format1,
				 format2, stream ) ) != HB_Err_Ok )
      goto Fail;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
    Free_PairSet( &ps[m], format1, format2 );

  FREE( ps );
  return error;
}

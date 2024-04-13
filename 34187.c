static HB_Error  Load_PairSet ( HB_PairSet*  ps,
				HB_UShort     format1,
				HB_UShort     format2,
				HB_Stream     stream )
{
  HB_Error  error;

  HB_UShort             n, m, count;
  HB_UInt              base_offset;

  HB_PairValueRecord*  pvr;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = ps->PairValueCount = GET_UShort();

  FORGET_Frame();

  ps->PairValueRecord = NULL;

  if ( ALLOC_ARRAY( ps->PairValueRecord, count, HB_PairValueRecord ) )
    return error;

  pvr = ps->PairValueRecord;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    pvr[n].SecondGlyph = GET_UShort();

    FORGET_Frame();

    if ( format1 )
    {
      error = Load_ValueRecord( &pvr[n].Value1, format1,
				base_offset, stream );
      if ( error )
	goto Fail;
    }
    if ( format2 )
    {
      error = Load_ValueRecord( &pvr[n].Value2, format2,
				base_offset, stream );
      if ( error )
      {
	if ( format1 )
	  Free_ValueRecord( &pvr[n].Value1, format1 );
	goto Fail;
      }
    }
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
  {
    if ( format1 )
      Free_ValueRecord( &pvr[m].Value1, format1 );
    if ( format2 )
      Free_ValueRecord( &pvr[m].Value2, format2 );
  }

  FREE( pvr );
  return error;
}

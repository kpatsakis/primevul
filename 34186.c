static HB_Error  Load_PairPos2( HB_PairPosFormat2*  ppf2,
				HB_UShort            format1,
				HB_UShort            format2,
				HB_Stream            stream )
{
  HB_Error  error;

  HB_UShort          m, n, k, count1, count2;
  HB_UInt           cur_offset, new_offset1, new_offset2, base_offset;

  HB_Class1Record*  c1r;
  HB_Class2Record*  c2r;


  base_offset = FILE_Pos() - 8L;

  if ( ACCESS_Frame( 8L ) )
    return error;

  new_offset1 = GET_UShort() + base_offset;
  new_offset2 = GET_UShort() + base_offset;

  /* `Class1Count' and `Class2Count' are the upper limits for class
     values, thus we read it now to make additional safety checks.  */

  count1 = ppf2->Class1Count = GET_UShort();
  count2 = ppf2->Class2Count = GET_UShort();

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset1 ) ||
       ( error = _HB_OPEN_Load_ClassDefinition( &ppf2->ClassDef1, count1,
				       stream ) ) != HB_Err_Ok )
    return error;
  if ( FILE_Seek( new_offset2 ) ||
       ( error = _HB_OPEN_Load_ClassDefinition( &ppf2->ClassDef2, count2,
				       stream ) ) != HB_Err_Ok )
    goto Fail3;
  (void)FILE_Seek( cur_offset );

  ppf2->Class1Record = NULL;

  if ( ALLOC_ARRAY( ppf2->Class1Record, count1, HB_Class1Record ) )
    goto Fail2;

  c1r = ppf2->Class1Record;

  for ( m = 0; m < count1; m++ )
  {
    c1r[m].Class2Record = NULL;

    if ( ALLOC_ARRAY( c1r[m].Class2Record, count2, HB_Class2Record ) )
      goto Fail1;

    c2r = c1r[m].Class2Record;

    for ( n = 0; n < count2; n++ )
    {
      if ( format1 )
      {
	error = Load_ValueRecord( &c2r[n].Value1, format1,
				  base_offset, stream );
	if ( error )
	  goto Fail0;
      }
      if ( format2 )
      {
	error = Load_ValueRecord( &c2r[n].Value2, format2,
				  base_offset, stream );
	if ( error )
	{
	  if ( format1 )
	    Free_ValueRecord( &c2r[n].Value1, format1 );
	  goto Fail0;
	}
      }
    }

    continue;

  Fail0:
    for ( k = 0; k < n; k++ )
    {
      if ( format1 )
	Free_ValueRecord( &c2r[k].Value1, format1 );
      if ( format2 )
	Free_ValueRecord( &c2r[k].Value2, format2 );
    }
    goto Fail1;
  }

  return HB_Err_Ok;

Fail1:
  for ( k = 0; k < m; k++ )
  {
    c2r = c1r[k].Class2Record;

    for ( n = 0; n < count2; n++ )
    {
      if ( format1 )
	Free_ValueRecord( &c2r[n].Value1, format1 );
      if ( format2 )
	Free_ValueRecord( &c2r[n].Value2, format2 );
    }

    FREE( c2r );
  }

  FREE( c1r );
Fail2:

  _HB_OPEN_Free_ClassDefinition( &ppf2->ClassDef2 );

Fail3:
  _HB_OPEN_Free_ClassDefinition( &ppf2->ClassDef1 );
  return error;
}

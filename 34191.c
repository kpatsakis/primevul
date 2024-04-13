static HB_Error  Load_SinglePos( HB_GPOS_SubTable* st,
				 HB_Stream       stream )
{
  HB_Error  error;
  HB_SinglePos*   sp = &st->single;

  HB_UShort         n, m, count, format;
  HB_UInt          cur_offset, new_offset, base_offset;

  HB_ValueRecord*  vr;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 6L ) )
    return error;

  sp->PosFormat = GET_UShort();
  new_offset    = GET_UShort() + base_offset;

  format = sp->ValueFormat = GET_UShort();

  FORGET_Frame();

  if ( !format )
    return ERR(HB_Err_Invalid_SubTable);

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &sp->Coverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  switch ( sp->PosFormat )
  {
  case 1:
    error = Load_ValueRecord( &sp->spf.spf1.Value, format,
			      base_offset, stream );
    if ( error )
      goto Fail2;
    break;

  case 2:
    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    count = sp->spf.spf2.ValueCount = GET_UShort();

    FORGET_Frame();

    sp->spf.spf2.Value = NULL;

    if ( ALLOC_ARRAY( sp->spf.spf2.Value, count, HB_ValueRecord ) )
      goto Fail2;

    vr = sp->spf.spf2.Value;

    for ( n = 0; n < count; n++ )
    {
      error = Load_ValueRecord( &vr[n], format, base_offset, stream );
      if ( error )
	goto Fail1;
    }
    break;

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

  return HB_Err_Ok;

Fail1:
  for ( m = 0; m < n; m++ )
    Free_ValueRecord( &vr[m], format );

  FREE( vr );

Fail2:
  _HB_OPEN_Free_Coverage( &sp->Coverage );
  return error;
}

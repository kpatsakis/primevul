static HB_Error  Load_PairPos( HB_GPOS_SubTable* st,
			       HB_Stream     stream )
{
  HB_Error  error;
  HB_PairPos*     pp = &st->pair;

  HB_UShort         format1, format2;
  HB_UInt          cur_offset, new_offset, base_offset;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 8L ) )
    return error;

  pp->PosFormat = GET_UShort();
  new_offset    = GET_UShort() + base_offset;

  format1 = pp->ValueFormat1 = GET_UShort();
  format2 = pp->ValueFormat2 = GET_UShort();

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &pp->Coverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  switch ( pp->PosFormat )
  {
  case 1:
    error = Load_PairPos1( &pp->ppf.ppf1, format1, format2, stream );
    if ( error )
      goto Fail;
    break;

  case 2:
    error = Load_PairPos2( &pp->ppf.ppf2, format1, format2, stream );
    if ( error )
      goto Fail;
    break;

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

  return HB_Err_Ok;

Fail:
  _HB_OPEN_Free_Coverage( &pp->Coverage );
  return error;
}

static HB_Error  Load_MarkMarkPos( HB_GPOS_SubTable* st,
				   HB_Stream         stream )
{
  HB_Error  error;
  HB_MarkMarkPos* mmp = &st->markmark;

  HB_UInt  cur_offset, new_offset, base_offset;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 4L ) )
    return error;

  mmp->PosFormat = GET_UShort();
  new_offset     = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &mmp->Mark1Coverage,
				stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &mmp->Mark2Coverage,
				stream ) ) != HB_Err_Ok )
    goto Fail3;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 4L ) )
    goto Fail2;

  mmp->ClassCount = GET_UShort();
  new_offset      = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = Load_MarkArray( &mmp->Mark1Array, stream ) ) != HB_Err_Ok )
    goto Fail2;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail1;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = Load_Mark2Array( &mmp->Mark2Array, mmp->ClassCount,
				  stream ) ) != HB_Err_Ok )
    goto Fail1;

  return HB_Err_Ok;

Fail1:
  Free_MarkArray( &mmp->Mark1Array );

Fail2:
  _HB_OPEN_Free_Coverage( &mmp->Mark2Coverage );

Fail3:
  _HB_OPEN_Free_Coverage( &mmp->Mark1Coverage );
  return error;
}

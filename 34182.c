static HB_Error  Load_MarkBasePos( HB_GPOS_SubTable* st,
				   HB_Stream         stream )
{
  HB_Error  error;
  HB_MarkBasePos* mbp = &st->markbase;

  HB_UInt  cur_offset, new_offset, base_offset;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 4L ) )
    return error;

  mbp->PosFormat = GET_UShort();
  new_offset     = GET_UShort() + base_offset;

  FORGET_Frame();

  if (mbp->PosFormat != 1)
    return ERR(HB_Err_Invalid_SubTable_Format);

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &mbp->MarkCoverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &mbp->BaseCoverage, stream ) ) != HB_Err_Ok )
    goto Fail3;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 4L ) )
    goto Fail2;

  mbp->ClassCount = GET_UShort();
  new_offset      = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = Load_MarkArray( &mbp->MarkArray, stream ) ) != HB_Err_Ok )
    goto Fail2;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail1;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = Load_BaseArray( &mbp->BaseArray, mbp->ClassCount,
				 stream ) ) != HB_Err_Ok )
    goto Fail1;

  return HB_Err_Ok;

Fail1:
  Free_MarkArray( &mbp->MarkArray );

Fail2:
  _HB_OPEN_Free_Coverage( &mbp->BaseCoverage );

Fail3:
  _HB_OPEN_Free_Coverage( &mbp->MarkCoverage );
  return error;
}

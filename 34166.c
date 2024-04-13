static HB_Error  Load_BaseArray( HB_BaseArray*  ba,
				 HB_UShort       num_classes,
				 HB_Stream       stream )
{
  HB_Error  error;

  HB_UShort       m, n, count;
  HB_UInt         cur_offset, new_offset, base_offset;

  HB_BaseRecord  *br;
  HB_Anchor      *ban, *bans;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = ba->BaseCount = GET_UShort();

  FORGET_Frame();

  ba->BaseRecord = NULL;

  if ( ALLOC_ARRAY( ba->BaseRecord, count, HB_BaseRecord ) )
    return error;

  br = ba->BaseRecord;

  bans = NULL;

  if ( ALLOC_ARRAY( bans, count * num_classes, HB_Anchor ) )
    goto Fail;

  for ( m = 0; m < count; m++ )
  {
    br[m].BaseAnchor = NULL;

    ban = br[m].BaseAnchor = bans + m * num_classes;

    for ( n = 0; n < num_classes; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
	goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      if (new_offset == base_offset) {
	/* XXX
	 * Doulos SIL Regular is buggy and has zero offsets here.
	 * Skip it
	 */
	ban[n].PosFormat = 0;
	continue;
      }

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = Load_Anchor( &ban[n], stream ) ) != HB_Err_Ok )
	goto Fail;
      (void)FILE_Seek( cur_offset );
    }
  }

  return HB_Err_Ok;

Fail:
  FREE( bans );
  FREE( br );
  return error;
}

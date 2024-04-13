static HB_Error  Load_ChainPosRuleSet( HB_ChainPosRuleSet*  cprs,
				       HB_Stream             stream )
{
  HB_Error  error;

  HB_UShort          n, m, count;
  HB_UInt           cur_offset, new_offset, base_offset;

  HB_ChainPosRule*  cpr;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = cprs->ChainPosRuleCount = GET_UShort();

  FORGET_Frame();

  cprs->ChainPosRule = NULL;

  if ( ALLOC_ARRAY( cprs->ChainPosRule, count, HB_ChainPosRule ) )
    return error;

  cpr = cprs->ChainPosRule;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_ChainPosRule( &cpr[n], stream ) ) != HB_Err_Ok )
      goto Fail;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
    Free_ChainPosRule( &cpr[m] );

  FREE( cpr );
  return error;
}

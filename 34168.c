static HB_Error  Load_ChainContextPos1( HB_ChainContextPosFormat1*  ccpf1,
					HB_Stream                    stream )
{
  HB_Error  error;

  HB_UShort             n, m, count;
  HB_UInt              cur_offset, new_offset, base_offset;

  HB_ChainPosRuleSet*  cprs;


  base_offset = FILE_Pos() - 2L;

  if ( ACCESS_Frame( 2L ) )
    return error;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &ccpf1->Coverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  count = ccpf1->ChainPosRuleSetCount = GET_UShort();

  FORGET_Frame();

  ccpf1->ChainPosRuleSet = NULL;

  if ( ALLOC_ARRAY( ccpf1->ChainPosRuleSet, count, HB_ChainPosRuleSet ) )
    goto Fail2;

  cprs = ccpf1->ChainPosRuleSet;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_ChainPosRuleSet( &cprs[n], stream ) ) != HB_Err_Ok )
      goto Fail1;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail1:
  for ( m = 0; m < n; m++ )
    Free_ChainPosRuleSet( &cprs[m] );

  FREE( cprs );

Fail2:
  _HB_OPEN_Free_Coverage( &ccpf1->Coverage );
  return error;
}

static HB_Error  Load_ContextPos1( HB_ContextPosFormat1*  cpf1,
				   HB_Stream               stream )
{
  HB_Error  error;

  HB_UShort        n, m, count;
  HB_UInt         cur_offset, new_offset, base_offset;

  HB_PosRuleSet*  prs;


  base_offset = FILE_Pos() - 2L;

  if ( ACCESS_Frame( 2L ) )
    return error;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &cpf1->Coverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  count = cpf1->PosRuleSetCount = GET_UShort();

  FORGET_Frame();

  cpf1->PosRuleSet = NULL;

  if ( ALLOC_ARRAY( cpf1->PosRuleSet, count, HB_PosRuleSet ) )
    goto Fail2;

  prs = cpf1->PosRuleSet;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_PosRuleSet( &prs[n], stream ) ) != HB_Err_Ok )
      goto Fail1;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail1:
  for ( m = 0; m < n; m++ )
    Free_PosRuleSet( &prs[m] );

  FREE( prs );

Fail2:
  _HB_OPEN_Free_Coverage( &cpf1->Coverage );
  return error;
}

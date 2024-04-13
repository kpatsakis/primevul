static HB_Error  Load_PosClassSet( HB_ContextPosFormat2*  cpf2,
				   HB_PosClassSet*        pcs,
				   HB_Stream               stream )
{
  HB_Error  error;

  HB_UShort          n, m, count;
  HB_UInt           cur_offset, new_offset, base_offset;

  HB_PosClassRule*  pcr;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = pcs->PosClassRuleCount = GET_UShort();

  FORGET_Frame();

  pcs->PosClassRule = NULL;

  if ( ALLOC_ARRAY( pcs->PosClassRule, count, HB_PosClassRule ) )
    return error;

  pcr = pcs->PosClassRule;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_PosClassRule( cpf2, &pcr[n],
				      stream ) ) != HB_Err_Ok )
      goto Fail;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
    Free_PosClassRule( &pcr[m] );

  FREE( pcr );
  return error;
}

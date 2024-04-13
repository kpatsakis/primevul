static HB_Error  Load_ChainPosClassSet(
		   HB_ChainContextPosFormat2*  ccpf2,
		   HB_ChainPosClassSet*        cpcs,
		   HB_Stream                    stream )
{
  HB_Error  error;

  HB_UShort               n, m, count;
  HB_UInt                cur_offset, new_offset, base_offset;

  HB_ChainPosClassRule*  cpcr;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = cpcs->ChainPosClassRuleCount = GET_UShort();

  FORGET_Frame();

  cpcs->ChainPosClassRule = NULL;

  if ( ALLOC_ARRAY( cpcs->ChainPosClassRule, count,
		    HB_ChainPosClassRule ) )
    return error;

  cpcr = cpcs->ChainPosClassRule;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_ChainPosClassRule( ccpf2, &cpcr[n],
					   stream ) ) != HB_Err_Ok )
      goto Fail;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
    Free_ChainPosClassRule( &cpcr[m] );

  FREE( cpcr );
  return error;
}

static HB_Error  Load_ChainContextPos( HB_GPOS_SubTable* st,
				       HB_Stream             stream )
{
  HB_Error  error;
  HB_ChainContextPos*  ccp = &st->chain;


  if ( ACCESS_Frame( 2L ) )
    return error;

  ccp->PosFormat = GET_UShort();

  FORGET_Frame();

  switch ( ccp->PosFormat )
  {
  case 1:
    return Load_ChainContextPos1( &ccp->ccpf.ccpf1, stream );

  case 2:
    return Load_ChainContextPos2( &ccp->ccpf.ccpf2, stream );

  case 3:
    return Load_ChainContextPos3( &ccp->ccpf.ccpf3, stream );

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

  return HB_Err_Ok;               /* never reached */
}

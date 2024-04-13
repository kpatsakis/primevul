static HB_Error  Lookup_ChainContextPos(
		   GPOS_Instance*        gpi,
		   HB_GPOS_SubTable* st,
		   HB_Buffer            buffer,
		   HB_UShort             flags,
		   HB_UShort             context_length,
		   int                   nesting_level )
{
  HB_ChainContextPos*  ccp = &st->chain;

  switch ( ccp->PosFormat )
  {
  case 1:
    return Lookup_ChainContextPos1( gpi, &ccp->ccpf.ccpf1, buffer,
				    flags, context_length,
				    nesting_level );

  case 2:
    return Lookup_ChainContextPos2( gpi, &ccp->ccpf.ccpf2, buffer,
				    flags, context_length,
				    nesting_level );

  case 3:
    return Lookup_ChainContextPos3( gpi, &ccp->ccpf.ccpf3, buffer,
				    flags, context_length,
				    nesting_level );

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

  return HB_Err_Ok;               /* never reached */
}

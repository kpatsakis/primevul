static HB_Error  Lookup_ContextPos( GPOS_Instance*    gpi,
				    HB_GPOS_SubTable* st,
				    HB_Buffer        buffer,
				    HB_UShort         flags,
				    HB_UShort         context_length,
				    int               nesting_level )
{
  HB_ContextPos*   cp = &st->context;

  switch ( cp->PosFormat )
  {
  case 1:
    return Lookup_ContextPos1( gpi, &cp->cpf.cpf1, buffer,
			       flags, context_length, nesting_level );

  case 2:
    return Lookup_ContextPos2( gpi, &cp->cpf.cpf2, buffer,
			       flags, context_length, nesting_level );

  case 3:
    return Lookup_ContextPos3( gpi, &cp->cpf.cpf3, buffer,
			       flags, context_length, nesting_level );

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

  return HB_Err_Ok;               /* never reached */
}

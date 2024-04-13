static HB_Error  Lookup_ContextPos3( GPOS_Instance*          gpi,
				     HB_ContextPosFormat3*  cpf3,
				     HB_Buffer              buffer,
				     HB_UShort               flags,
				     HB_UShort               context_length,
				     int                     nesting_level )
{
  HB_Error         error;
  HB_UShort        index, i, j, property;
  HB_GPOSHeader*  gpos = gpi->gpos;

  HB_Coverage*    c;
  HB_GDEFHeader*  gdef;


  gdef = gpos->gdef;

  if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
    return error;

  if ( context_length != 0xFFFF && context_length < cpf3->GlyphCount )
    return HB_Err_Not_Covered;

  if ( buffer->in_pos + cpf3->GlyphCount > buffer->in_length )
    return HB_Err_Not_Covered;         /* context is too long */

  c    = cpf3->Coverage;

  for ( i = 1, j = 1; i < cpf3->GlyphCount; i++, j++ )
  {
    while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
    {
      if ( error && error != HB_Err_Not_Covered )
	return error;

      if ( j + cpf3->GlyphCount - i == (HB_Int)buffer->in_length )
	return HB_Err_Not_Covered;
      j++;
    }

    error = _HB_OPEN_Coverage_Index( &c[i], IN_GLYPH( j ), &index );
    if ( error )
      return error;
  }

  return Do_ContextPos( gpi, cpf3->GlyphCount,
			cpf3->PosCount, cpf3->PosLookupRecord,
			buffer,
			nesting_level );
}

static HB_Error  Lookup_ChainContextPos3(
		   GPOS_Instance*               gpi,
		   HB_ChainContextPosFormat3*  ccpf3,
		   HB_Buffer                   buffer,
		   HB_UShort                    flags,
		   HB_UShort                    context_length,
		   int                          nesting_level )
{
  HB_UShort        index, i, j, property;
  HB_UShort        bgc, igc, lgc;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;

  HB_Coverage*    bc;
  HB_Coverage*    ic;
  HB_Coverage*    lc;
  HB_GDEFHeader*  gdef;


  gdef = gpos->gdef;

  if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
    return error;

  bgc = ccpf3->BacktrackGlyphCount;
  igc = ccpf3->InputGlyphCount;
  lgc = ccpf3->LookaheadGlyphCount;

  if ( context_length != 0xFFFF && context_length < igc )
    return HB_Err_Not_Covered;

  /* check whether context is too long; it is a first guess only */

  if ( bgc > buffer->in_pos || buffer->in_pos + igc + lgc > buffer->in_length )
    return HB_Err_Not_Covered;

  if ( bgc )
  {
    /* Since we don't know in advance the number of glyphs to inspect,
       we search backwards for matches in the backtrack glyph array    */

    bc       = ccpf3->BacktrackCoverage;

    for ( i = 0, j = buffer->in_pos - 1; i < bgc; i++, j-- )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  return error;

	if ( j + 1 == bgc - i )
	  return HB_Err_Not_Covered;
	j--;
      }

      error = _HB_OPEN_Coverage_Index( &bc[i], IN_GLYPH( j ), &index );
      if ( error )
	return error;
    }
  }

  ic       = ccpf3->InputCoverage;

  for ( i = 0, j = buffer->in_pos; i < igc; i++, j++ )
  {
    /* We already called CHECK_Property for IN_GLYPH ( buffer->in_pos ) */
    while ( j > buffer->in_pos && CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
    {
      if ( error && error != HB_Err_Not_Covered )
	return error;

      if ( j + igc - i + lgc == (HB_Int)buffer->in_length )
	return HB_Err_Not_Covered;
      j++;
    }

    error = _HB_OPEN_Coverage_Index( &ic[i], IN_GLYPH( j ), &index );
    if ( error )
      return error;
  }

  /* we are starting to check for lookahead glyphs right after the
     last context glyph                                            */

  lc       = ccpf3->LookaheadCoverage;

  for ( i = 0; i < lgc; i++, j++ )
  {
    while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
    {
      if ( error && error != HB_Err_Not_Covered )
	return error;

      if ( j + lgc - i == (HB_Int)buffer->in_length )
	return HB_Err_Not_Covered;
      j++;
    }

    error = _HB_OPEN_Coverage_Index( &lc[i], IN_GLYPH( j ), &index );
    if ( error )
      return error;
  }

  return Do_ContextPos( gpi, igc,
			ccpf3->PosCount,
			ccpf3->PosLookupRecord,
			buffer,
			nesting_level );
}

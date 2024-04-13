static HB_Error  GPOS_Do_Glyph_Lookup( GPOS_Instance*    gpi,
				       HB_UShort         lookup_index,
				       HB_Buffer        buffer,
				       HB_UShort         context_length,
				       int               nesting_level )
{
  HB_Error             error = HB_Err_Not_Covered;
  HB_UShort            i, flags, lookup_count;
  HB_GPOSHeader*       gpos = gpi->gpos;
  HB_Lookup*           lo;
  int		       lookup_type;


  nesting_level++;

  if ( nesting_level > HB_MAX_NESTING_LEVEL )
    return ERR(HB_Err_Not_Covered); /* ERR() call intended */

  lookup_count = gpos->LookupList.LookupCount;
  if (lookup_index >= lookup_count)
    return error;

  lo    = &gpos->LookupList.Lookup[lookup_index];
  flags = lo->LookupFlag;
  lookup_type = lo->LookupType;

  for ( i = 0; i < lo->SubTableCount; i++ )
  {
    HB_GPOS_SubTable *st = &lo->SubTable[i].st.gpos;

    switch (lookup_type) {
      case HB_GPOS_LOOKUP_SINGLE:
        error = Lookup_SinglePos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_PAIR:
	error = Lookup_PairPos		( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_CURSIVE:
	error = Lookup_CursivePos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_MARKBASE:
	error = Lookup_MarkBasePos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_MARKLIG:
	error = Lookup_MarkLigPos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_MARKMARK:
	error = Lookup_MarkMarkPos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_CONTEXT:
	error = Lookup_ContextPos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
      case HB_GPOS_LOOKUP_CHAIN:
	error = Lookup_ChainContextPos	( gpi, st, buffer, flags, context_length, nesting_level ); break;
    /*case HB_GPOS_LOOKUP_EXTENSION:
	error = Lookup_ExtensionPos	( gpi, st, buffer, flags, context_length, nesting_level ); break;*/
      default:
	error = HB_Err_Not_Covered;
    }

    /* Check whether we have a successful positioning or an error other
       than HB_Err_Not_Covered                                         */
    if ( error != HB_Err_Not_Covered )
      return error;
  }

  return HB_Err_Not_Covered;
}

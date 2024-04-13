_HB_GPOS_Load_SubTable( HB_GPOS_SubTable* st,
			HB_Stream         stream,
			HB_UShort         lookup_type )
{
  switch ( lookup_type ) {
    case HB_GPOS_LOOKUP_SINGLE:		return Load_SinglePos		( st, stream );
    case HB_GPOS_LOOKUP_PAIR:		return Load_PairPos		( st, stream );
    case HB_GPOS_LOOKUP_CURSIVE:	return Load_CursivePos		( st, stream );
    case HB_GPOS_LOOKUP_MARKBASE:	return Load_MarkBasePos		( st, stream );
    case HB_GPOS_LOOKUP_MARKLIG:	return Load_MarkLigPos		( st, stream );
    case HB_GPOS_LOOKUP_MARKMARK:	return Load_MarkMarkPos		( st, stream );
    case HB_GPOS_LOOKUP_CONTEXT:	return Load_ContextPos		( st, stream );
    case HB_GPOS_LOOKUP_CHAIN:		return Load_ChainContextPos	( st, stream );
  /*case HB_GPOS_LOOKUP_EXTENSION:	return Load_ExtensionPos	( st, stream );*/
    default:				return ERR(HB_Err_Invalid_SubTable_Format);
  }
}

static HB_Error  Lookup_SinglePos( GPOS_Instance*    gpi,
				   HB_GPOS_SubTable* st,
				   HB_Buffer        buffer,
				   HB_UShort         flags,
				   HB_UShort         context_length,
				   int               nesting_level )
{
  HB_UShort        index, property;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_SinglePos*   sp = &st->single;

  HB_UNUSED(nesting_level);

  if ( context_length != 0xFFFF && context_length < 1 )
    return HB_Err_Not_Covered;

  if ( CHECK_Property( gpos->gdef, IN_CURITEM(), flags, &property ) )
    return error;

  error = _HB_OPEN_Coverage_Index( &sp->Coverage, IN_CURGLYPH(), &index );
  if ( error )
    return error;

  switch ( sp->PosFormat )
  {
  case 1:
    error = Get_ValueRecord( gpi, &sp->spf.spf1.Value,
			     sp->ValueFormat, POSITION( buffer->in_pos ) );
    if ( error )
      return error;
    break;

  case 2:
    if ( index >= sp->spf.spf2.ValueCount )
      return ERR(HB_Err_Invalid_SubTable);
    error = Get_ValueRecord( gpi, &sp->spf.spf2.Value[index],
			     sp->ValueFormat, POSITION( buffer->in_pos ) );
    if ( error )
      return error;
    break;

  default:
    return ERR(HB_Err_Invalid_SubTable);
  }

  (buffer->in_pos)++;

  return HB_Err_Ok;
}

static HB_Error  Lookup_MarkBasePos( GPOS_Instance*    gpi,
				     HB_GPOS_SubTable* st,
				     HB_Buffer        buffer,
				     HB_UShort         flags,
				     HB_UShort         context_length,
				     int               nesting_level )
{
  HB_UShort        i, j, mark_index, base_index, property, class;
  HB_Fixed           x_mark_value, y_mark_value, x_base_value, y_base_value;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_MarkBasePos* mbp = &st->markbase;

  HB_MarkArray*   ma;
  HB_BaseArray*   ba;
  HB_BaseRecord*  br;
  HB_Anchor*      mark_anchor;
  HB_Anchor*      base_anchor;

  HB_Position     o;

  HB_UNUSED(nesting_level);

  if ( context_length != 0xFFFF && context_length < 1 )
    return HB_Err_Not_Covered;

  if ( flags & HB_LOOKUP_FLAG_IGNORE_BASE_GLYPHS )
    return HB_Err_Not_Covered;

  if ( CHECK_Property( gpos->gdef, IN_CURITEM(),
		       flags, &property ) )
    return error;

  error = _HB_OPEN_Coverage_Index( &mbp->MarkCoverage, IN_CURGLYPH(),
			  &mark_index );
  if ( error )
    return error;

  /* now we search backwards for a non-mark glyph */

  i = 1;
  j = buffer->in_pos - 1;

  while ( i <= buffer->in_pos )
  {
    error = HB_GDEF_Get_Glyph_Property( gpos->gdef, IN_GLYPH( j ),
					&property );
    if ( error )
      return error;

    if ( !( property == HB_GDEF_MARK || property & HB_LOOKUP_FLAG_IGNORE_SPECIAL_MARKS ) )
      break;

    i++;
    j--;
  }

  /* The following assertion is too strong -- at least for mangal.ttf. */
#if 0
  if ( property != HB_GDEF_BASE_GLYPH )
    return HB_Err_Not_Covered;
#endif

  if ( i > buffer->in_pos )
    return HB_Err_Not_Covered;

  error = _HB_OPEN_Coverage_Index( &mbp->BaseCoverage, IN_GLYPH( j ),
			  &base_index );
  if ( error )
    return error;

  ma = &mbp->MarkArray;

  if ( mark_index >= ma->MarkCount )
    return ERR(HB_Err_Invalid_SubTable);

  class       = ma->MarkRecord[mark_index].Class;
  mark_anchor = &ma->MarkRecord[mark_index].MarkAnchor;

  if ( class >= mbp->ClassCount )
    return ERR(HB_Err_Invalid_SubTable);

  ba = &mbp->BaseArray;

  if ( base_index >= ba->BaseCount )
    return ERR(HB_Err_Invalid_SubTable);

  br          = &ba->BaseRecord[base_index];
  base_anchor = &br->BaseAnchor[class];

  error = Get_Anchor( gpi, mark_anchor, IN_CURGLYPH(),
		      &x_mark_value, &y_mark_value );
  if ( error )
    return error;

  error = Get_Anchor( gpi, base_anchor, IN_GLYPH( j ),
		      &x_base_value, &y_base_value );
  if ( error )
    return error;

  /* anchor points are not cumulative */

  o = POSITION( buffer->in_pos );

  o->x_pos     = x_base_value - x_mark_value;
  o->y_pos     = y_base_value - y_mark_value;
  o->x_advance = 0;
  o->y_advance = 0;
  o->back      = i;

  (buffer->in_pos)++;

  return HB_Err_Ok;
}

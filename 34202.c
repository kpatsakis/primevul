static HB_Error  Lookup_MarkLigPos( GPOS_Instance*    gpi,
				    HB_GPOS_SubTable* st,
				    HB_Buffer        buffer,
				    HB_UShort         flags,
				    HB_UShort         context_length,
				    int               nesting_level )
{
  HB_UShort        i, j, mark_index, lig_index, property, class;
  HB_UShort        mark_glyph;
  HB_Fixed           x_mark_value, y_mark_value, x_lig_value, y_lig_value;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_MarkLigPos*  mlp = &st->marklig;

  HB_MarkArray*        ma;
  HB_LigatureArray*    la;
  HB_LigatureAttach*   lat;
  HB_ComponentRecord*  cr;
  HB_UShort             comp_index;
  HB_Anchor*           mark_anchor;
  HB_Anchor*           lig_anchor;

  HB_Position    o;

  HB_UNUSED(nesting_level);

  if ( context_length != 0xFFFF && context_length < 1 )
    return HB_Err_Not_Covered;

  if ( flags & HB_LOOKUP_FLAG_IGNORE_LIGATURES )
    return HB_Err_Not_Covered;

  mark_glyph = IN_CURGLYPH();

  if ( CHECK_Property( gpos->gdef, IN_CURITEM(), flags, &property ) )
    return error;

  error = _HB_OPEN_Coverage_Index( &mlp->MarkCoverage, mark_glyph, &mark_index );
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

  /* Similar to Lookup_MarkBasePos(), I suspect that this assertion is
     too strong, thus it is commented out.                             */
#if 0
  if ( property != HB_GDEF_LIGATURE )
    return HB_Err_Not_Covered;
#endif

  if ( i > buffer->in_pos )
    return HB_Err_Not_Covered;

  error = _HB_OPEN_Coverage_Index( &mlp->LigatureCoverage, IN_GLYPH( j ),
			  &lig_index );
  if ( error )
    return error;

  ma = &mlp->MarkArray;

  if ( mark_index >= ma->MarkCount )
    return ERR(HB_Err_Invalid_SubTable);

  class       = ma->MarkRecord[mark_index].Class;
  mark_anchor = &ma->MarkRecord[mark_index].MarkAnchor;

  if ( class >= mlp->ClassCount )
    return ERR(HB_Err_Invalid_SubTable);

  la = &mlp->LigatureArray;

  if ( lig_index >= la->LigatureCount )
    return ERR(HB_Err_Invalid_SubTable);

  lat = &la->LigatureAttach[lig_index];

  /* We must now check whether the ligature ID of the current mark glyph
     is identical to the ligature ID of the found ligature.  If yes, we
     can directly use the component index.  If not, we attach the mark
     glyph to the last component of the ligature.                        */

  if ( IN_LIGID( j ) == IN_LIGID( buffer->in_pos) )
  {
    comp_index = IN_COMPONENT( buffer->in_pos );
    if ( comp_index >= lat->ComponentCount )
      return HB_Err_Not_Covered;
  }
  else
    comp_index = lat->ComponentCount - 1;

  cr         = &lat->ComponentRecord[comp_index];
  lig_anchor = &cr->LigatureAnchor[class];

  error = Get_Anchor( gpi, mark_anchor, IN_CURGLYPH(),
		      &x_mark_value, &y_mark_value );
  if ( error )
    return error;
  error = Get_Anchor( gpi, lig_anchor, IN_GLYPH( j ),
		      &x_lig_value, &y_lig_value );
  if ( error )
    return error;

  /* anchor points are not cumulative */

  o = POSITION( buffer->in_pos );

  o->x_pos     = x_lig_value - x_mark_value;
  o->y_pos     = y_lig_value - y_mark_value;
  o->x_advance = 0;
  o->y_advance = 0;
  o->back      = i;

  (buffer->in_pos)++;

  return HB_Err_Ok;
}

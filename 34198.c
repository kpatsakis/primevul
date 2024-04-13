static HB_Error  Lookup_ContextPos2( GPOS_Instance*          gpi,
				     HB_ContextPosFormat2*  cpf2,
				     HB_Buffer              buffer,
				     HB_UShort               flags,
				     HB_UShort               context_length,
				     int                     nesting_level )
{
  HB_UShort          index, property;
  HB_Error           error;
  HB_UShort          i, j, k, known_classes;

  HB_UShort*         classes;
  HB_UShort*         cl;
  HB_GPOSHeader*    gpos = gpi->gpos;

  HB_PosClassSet*   pcs;
  HB_PosClassRule*  pr;
  HB_GDEFHeader*    gdef;


  gdef = gpos->gdef;

  if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
    return error;

  /* Note: The coverage table in format 2 doesn't give an index into
	   anything.  It just lets us know whether or not we need to
	   do any lookup at all.                                     */

  error = _HB_OPEN_Coverage_Index( &cpf2->Coverage, IN_CURGLYPH(), &index );
  if ( error )
    return error;

  if (cpf2->MaxContextLength < 1)
    return HB_Err_Not_Covered;

  if ( ALLOC_ARRAY( classes, cpf2->MaxContextLength, HB_UShort ) )
    return error;

  error = _HB_OPEN_Get_Class( &cpf2->ClassDef, IN_CURGLYPH(),
		     &classes[0], NULL );
  if ( error && error != HB_Err_Not_Covered )
    goto End;
  known_classes = 0;

  pcs = &cpf2->PosClassSet[classes[0]];
  if ( !pcs )
  {
    error = ERR(HB_Err_Invalid_SubTable);
    goto End;
  }

  for ( k = 0; k < pcs->PosClassRuleCount; k++ )
  {
    pr = &pcs->PosClassRule[k];

    if ( context_length != 0xFFFF && context_length < pr->GlyphCount )
      goto next_posclassrule;

    if ( buffer->in_pos + pr->GlyphCount > buffer->in_length )
      goto next_posclassrule;                /* context is too long */

    cl   = pr->Class;

    /* Start at 1 because [0] is implied */

    for ( i = 1, j = buffer->in_pos + 1; i < pr->GlyphCount; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  goto End;

	if ( j + pr->GlyphCount - i == (HB_Int)buffer->in_length )
	  goto next_posclassrule;
	j++;
      }

      if ( i > known_classes )
      {
	/* Keeps us from having to do this for each rule */

	error = _HB_OPEN_Get_Class( &cpf2->ClassDef, IN_GLYPH( j ), &classes[i], NULL );
	if ( error && error != HB_Err_Not_Covered )
	  goto End;
	known_classes = i;
      }

      if ( cl[i - 1] != classes[i] )
	goto next_posclassrule;
    }

    error = Do_ContextPos( gpi, pr->GlyphCount,
			   pr->PosCount, pr->PosLookupRecord,
			   buffer,
			   nesting_level );
    goto End;

  next_posclassrule:
    ;
  }

  error = HB_Err_Not_Covered;

End:
  FREE( classes );
  return error;
}

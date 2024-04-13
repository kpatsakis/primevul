static HB_Error  Lookup_ChainContextPos2(
		   GPOS_Instance*               gpi,
		   HB_ChainContextPosFormat2*  ccpf2,
		   HB_Buffer                   buffer,
		   HB_UShort                    flags,
		   HB_UShort                    context_length,
		   int                          nesting_level )
{
  HB_UShort              index, property;
  HB_Error               error;
  HB_UShort              i, j, k;
  HB_UShort              bgc, igc, lgc;
  HB_UShort              known_backtrack_classes,
			 known_input_classes,
			 known_lookahead_classes;

  HB_UShort*             backtrack_classes;
  HB_UShort*             input_classes;
  HB_UShort*             lookahead_classes;

  HB_UShort*             bc;
  HB_UShort*             ic;
  HB_UShort*             lc;
  HB_GPOSHeader*        gpos = gpi->gpos;

  HB_ChainPosClassSet*  cpcs;
  HB_ChainPosClassRule  cpcr;
  HB_GDEFHeader*        gdef;


  gdef = gpos->gdef;

  if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
    return error;

  /* Note: The coverage table in format 2 doesn't give an index into
	   anything.  It just lets us know whether or not we need to
	   do any lookup at all.                                     */

  error = _HB_OPEN_Coverage_Index( &ccpf2->Coverage, IN_CURGLYPH(), &index );
  if ( error )
    return error;

  if ( ALLOC_ARRAY( backtrack_classes, ccpf2->MaxBacktrackLength, HB_UShort ) )
    return error;
  known_backtrack_classes = 0;

  if (ccpf2->MaxInputLength < 1)
    return HB_Err_Not_Covered;

  if ( ALLOC_ARRAY( input_classes, ccpf2->MaxInputLength, HB_UShort ) )
    goto End3;
  known_input_classes = 1;

  if ( ALLOC_ARRAY( lookahead_classes, ccpf2->MaxLookaheadLength, HB_UShort ) )
    goto End2;
  known_lookahead_classes = 0;

  error = _HB_OPEN_Get_Class( &ccpf2->InputClassDef, IN_CURGLYPH(),
		     &input_classes[0], NULL );
  if ( error && error != HB_Err_Not_Covered )
    goto End1;

  cpcs = &ccpf2->ChainPosClassSet[input_classes[0]];
  if ( !cpcs )
  {
    error = ERR(HB_Err_Invalid_SubTable);
    goto End1;
  }

  for ( k = 0; k < cpcs->ChainPosClassRuleCount; k++ )
  {
    cpcr = cpcs->ChainPosClassRule[k];
    bgc  = cpcr.BacktrackGlyphCount;
    igc  = cpcr.InputGlyphCount;
    lgc  = cpcr.LookaheadGlyphCount;

    if ( context_length != 0xFFFF && context_length < igc )
      goto next_chainposclassrule;

    /* check whether context is too long; it is a first guess only */

    if ( bgc > buffer->in_pos || buffer->in_pos + igc + lgc > buffer->in_length )
      goto next_chainposclassrule;

    if ( bgc )
    {
      /* Since we don't know in advance the number of glyphs to inspect,
	 we search backwards for matches in the backtrack glyph array.
	 Note that `known_backtrack_classes' starts at index 0.         */

      bc       = cpcr.Backtrack;

      for ( i = 0, j = buffer->in_pos - 1; i < bgc; i++, j-- )
      {
	while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
	{
	  if ( error && error != HB_Err_Not_Covered )
	    goto End1;

	  if ( j + 1 == bgc - i )
	    goto next_chainposclassrule;
	  j++;
	}

	if ( i >= known_backtrack_classes )
	{
	  /* Keeps us from having to do this for each rule */

	  error = _HB_OPEN_Get_Class( &ccpf2->BacktrackClassDef, IN_GLYPH( j ),
			     &backtrack_classes[i], NULL );
	  if ( error && error != HB_Err_Not_Covered )
	    goto End1;
	  known_backtrack_classes = i;
	}

	if ( bc[i] != backtrack_classes[i] )
	  goto next_chainposclassrule;
      }
    }

    ic       = cpcr.Input;

    /* Start at 1 because [0] is implied */

    for ( i = 1, j = buffer->in_pos + 1; i < igc; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  goto End1;

	if ( j + igc - i + lgc == (HB_Int)buffer->in_length )
	  goto next_chainposclassrule;
	j++;
      }

      if ( i >= known_input_classes )
      {
	error = _HB_OPEN_Get_Class( &ccpf2->InputClassDef, IN_GLYPH( j ),
			   &input_classes[i], NULL );
	if ( error && error != HB_Err_Not_Covered )
	  goto End1;
	known_input_classes = i;
      }

      if ( ic[i - 1] != input_classes[i] )
	goto next_chainposclassrule;
    }

    /* we are starting to check for lookahead glyphs right after the
       last context glyph                                            */

    lc       = cpcr.Lookahead;

    for ( i = 0; i < lgc; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
	if ( error && error != HB_Err_Not_Covered )
	  goto End1;

	if ( j + lgc - i == (HB_Int)buffer->in_length )
	  goto next_chainposclassrule;
	j++;
      }

      if ( i >= known_lookahead_classes )
      {
	error = _HB_OPEN_Get_Class( &ccpf2->LookaheadClassDef, IN_GLYPH( j ),
			   &lookahead_classes[i], NULL );
	if ( error && error != HB_Err_Not_Covered )
	  goto End1;
	known_lookahead_classes = i;
      }

      if ( lc[i] != lookahead_classes[i] )
	goto next_chainposclassrule;
    }

    error = Do_ContextPos( gpi, igc,
			   cpcr.PosCount,
			   cpcr.PosLookupRecord,
			   buffer,
			   nesting_level );
    goto End1;

  next_chainposclassrule:
    ;
  }

  error = HB_Err_Not_Covered;

End1:
  FREE( lookahead_classes );

End2:
  FREE( input_classes );

End3:
  FREE( backtrack_classes );
  return error;
}

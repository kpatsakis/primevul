static HB_Error  Load_ChainPosClassRule(
		   HB_ChainContextPosFormat2*  ccpf2,
		   HB_ChainPosClassRule*       cpcr,
		   HB_Stream                    stream )
{
  HB_Error  error;

  HB_UShort             n, count;

  HB_UShort*            b;
  HB_UShort*            i;
  HB_UShort*            l;
  HB_PosLookupRecord*  plr;


  if ( ACCESS_Frame( 2L ) )
    return error;

  cpcr->BacktrackGlyphCount = GET_UShort();

  FORGET_Frame();

  if ( cpcr->BacktrackGlyphCount > ccpf2->MaxBacktrackLength )
    ccpf2->MaxBacktrackLength = cpcr->BacktrackGlyphCount;

  cpcr->Backtrack = NULL;

  count = cpcr->BacktrackGlyphCount;

  if ( ALLOC_ARRAY( cpcr->Backtrack, count, HB_UShort ) )
    return error;

  b = cpcr->Backtrack;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail4;

  for ( n = 0; n < count; n++ )
    b[n] = GET_UShort();

  FORGET_Frame();

  if ( ACCESS_Frame( 2L ) )
    goto Fail4;

  cpcr->InputGlyphCount = GET_UShort();

  if ( cpcr->InputGlyphCount > ccpf2->MaxInputLength )
    ccpf2->MaxInputLength = cpcr->InputGlyphCount;

  FORGET_Frame();

  cpcr->Input = NULL;

  count = cpcr->InputGlyphCount - 1; /* only InputGlyphCount - 1 elements */

  if ( ALLOC_ARRAY( cpcr->Input, count, HB_UShort ) )
    goto Fail4;

  i = cpcr->Input;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail3;

  for ( n = 0; n < count; n++ )
    i[n] = GET_UShort();

  FORGET_Frame();

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  cpcr->LookaheadGlyphCount = GET_UShort();

  FORGET_Frame();

  if ( cpcr->LookaheadGlyphCount > ccpf2->MaxLookaheadLength )
    ccpf2->MaxLookaheadLength = cpcr->LookaheadGlyphCount;

  cpcr->Lookahead = NULL;

  count = cpcr->LookaheadGlyphCount;

  if ( ALLOC_ARRAY( cpcr->Lookahead, count, HB_UShort ) )
    goto Fail3;

  l = cpcr->Lookahead;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail2;

  for ( n = 0; n < count; n++ )
    l[n] = GET_UShort();

  FORGET_Frame();

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  cpcr->PosCount = GET_UShort();

  FORGET_Frame();

  cpcr->PosLookupRecord = NULL;

  count = cpcr->PosCount;

  if ( ALLOC_ARRAY( cpcr->PosLookupRecord, count, HB_PosLookupRecord ) )
    goto Fail2;

  plr = cpcr->PosLookupRecord;

  if ( ACCESS_Frame( count * 4L ) )
    goto Fail1;

  for ( n = 0; n < count; n++ )
  {
    plr[n].SequenceIndex   = GET_UShort();
    plr[n].LookupListIndex = GET_UShort();
  }

  FORGET_Frame();

  return HB_Err_Ok;

Fail1:
  FREE( plr );

Fail2:
  FREE( l );

Fail3:
  FREE( i );

Fail4:
  FREE( b );
  return error;
}

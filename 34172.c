static HB_Error  Load_ChainPosRule( HB_ChainPosRule*  cpr,
				    HB_Stream          stream )
{
  HB_Error  error;

  HB_UShort             n, count;
  HB_UShort*            b;
  HB_UShort*            i;
  HB_UShort*            l;

  HB_PosLookupRecord*  plr;


  if ( ACCESS_Frame( 2L ) )
    return error;

  cpr->BacktrackGlyphCount = GET_UShort();

  FORGET_Frame();

  cpr->Backtrack = NULL;

  count = cpr->BacktrackGlyphCount;

  if ( ALLOC_ARRAY( cpr->Backtrack, count, HB_UShort ) )
    return error;

  b = cpr->Backtrack;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail4;

  for ( n = 0; n < count; n++ )
    b[n] = GET_UShort();

  FORGET_Frame();

  if ( ACCESS_Frame( 2L ) )
    goto Fail4;

  cpr->InputGlyphCount = GET_UShort();

  FORGET_Frame();

  cpr->Input = NULL;

  count = cpr->InputGlyphCount - 1;  /* only InputGlyphCount - 1 elements */

  if ( ALLOC_ARRAY( cpr->Input, count, HB_UShort ) )
    goto Fail4;

  i = cpr->Input;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail3;

  for ( n = 0; n < count; n++ )
    i[n] = GET_UShort();

  FORGET_Frame();

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  cpr->LookaheadGlyphCount = GET_UShort();

  FORGET_Frame();

  cpr->Lookahead = NULL;

  count = cpr->LookaheadGlyphCount;

  if ( ALLOC_ARRAY( cpr->Lookahead, count, HB_UShort ) )
    goto Fail3;

  l = cpr->Lookahead;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail2;

  for ( n = 0; n < count; n++ )
    l[n] = GET_UShort();

  FORGET_Frame();

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  cpr->PosCount = GET_UShort();

  FORGET_Frame();

  cpr->PosLookupRecord = NULL;

  count = cpr->PosCount;

  if ( ALLOC_ARRAY( cpr->PosLookupRecord, count, HB_PosLookupRecord ) )
    goto Fail2;

  plr = cpr->PosLookupRecord;

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

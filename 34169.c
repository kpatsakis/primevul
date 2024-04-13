static HB_Error  Load_ChainContextPos3( HB_ChainContextPosFormat3*  ccpf3,
					HB_Stream                    stream )
{
  HB_Error  error;

  HB_UShort             n, nb, ni, nl, m, count;
  HB_UShort             backtrack_count, input_count, lookahead_count;
  HB_UInt              cur_offset, new_offset, base_offset;

  HB_Coverage*         b;
  HB_Coverage*         i;
  HB_Coverage*         l;
  HB_PosLookupRecord*  plr;


  base_offset = FILE_Pos() - 2L;

  if ( ACCESS_Frame( 2L ) )
    return error;

  ccpf3->BacktrackGlyphCount = GET_UShort();

  FORGET_Frame();

  ccpf3->BacktrackCoverage = NULL;

  backtrack_count = ccpf3->BacktrackGlyphCount;

  if ( ALLOC_ARRAY( ccpf3->BacktrackCoverage, backtrack_count,
		    HB_Coverage ) )
    return error;

  b = ccpf3->BacktrackCoverage;

  for ( nb = 0; nb < backtrack_count; nb++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail4;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = _HB_OPEN_Load_Coverage( &b[nb], stream ) ) != HB_Err_Ok )
      goto Fail4;
    (void)FILE_Seek( cur_offset );
  }

  if ( ACCESS_Frame( 2L ) )
    goto Fail4;

  ccpf3->InputGlyphCount = GET_UShort();

  FORGET_Frame();

  ccpf3->InputCoverage = NULL;

  input_count = ccpf3->InputGlyphCount;

  if ( ALLOC_ARRAY( ccpf3->InputCoverage, input_count, HB_Coverage ) )
    goto Fail4;

  i = ccpf3->InputCoverage;

  for ( ni = 0; ni < input_count; ni++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail3;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = _HB_OPEN_Load_Coverage( &i[ni], stream ) ) != HB_Err_Ok )
      goto Fail3;
    (void)FILE_Seek( cur_offset );
  }

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  ccpf3->LookaheadGlyphCount = GET_UShort();

  FORGET_Frame();

  ccpf3->LookaheadCoverage = NULL;

  lookahead_count = ccpf3->LookaheadGlyphCount;

  if ( ALLOC_ARRAY( ccpf3->LookaheadCoverage, lookahead_count,
		    HB_Coverage ) )
    goto Fail3;

  l = ccpf3->LookaheadCoverage;

  for ( nl = 0; nl < lookahead_count; nl++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = _HB_OPEN_Load_Coverage( &l[nl], stream ) ) != HB_Err_Ok )
      goto Fail2;
    (void)FILE_Seek( cur_offset );
  }

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  ccpf3->PosCount = GET_UShort();

  FORGET_Frame();

  ccpf3->PosLookupRecord = NULL;

  count = ccpf3->PosCount;

  if ( ALLOC_ARRAY( ccpf3->PosLookupRecord, count, HB_PosLookupRecord ) )
    goto Fail2;

  plr = ccpf3->PosLookupRecord;

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
  for ( m = 0; m < nl; m++ )
    _HB_OPEN_Free_Coverage( &l[m] );

  FREE( l );

Fail3:
  for ( m = 0; m < ni; m++ )
    _HB_OPEN_Free_Coverage( &i[m] );

  FREE( i );

Fail4:
  for ( m = 0; m < nb; m++ )
    _HB_OPEN_Free_Coverage( &b[m] );

  FREE( b );
  return error;
}

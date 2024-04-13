static void  Free_ChainContextPos3( HB_ChainContextPosFormat3*  ccpf3 )
{
  HB_UShort      n, count;

  HB_Coverage*  c;


  FREE( ccpf3->PosLookupRecord );

  if ( ccpf3->LookaheadCoverage )
  {
    count = ccpf3->LookaheadGlyphCount;
    c     = ccpf3->LookaheadCoverage;

    for ( n = 0; n < count; n++ )
      _HB_OPEN_Free_Coverage( &c[n] );

    FREE( c );
  }

  if ( ccpf3->InputCoverage )
  {
    count = ccpf3->InputGlyphCount;
    c     = ccpf3->InputCoverage;

    for ( n = 0; n < count; n++ )
      _HB_OPEN_Free_Coverage( &c[n] );

    FREE( c );
  }

  if ( ccpf3->BacktrackCoverage )
  {
    count = ccpf3->BacktrackGlyphCount;
    c     = ccpf3->BacktrackCoverage;

    for ( n = 0; n < count; n++ )
      _HB_OPEN_Free_Coverage( &c[n] );

    FREE( c );
  }
}

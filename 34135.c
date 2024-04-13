static void  Free_ChainPosRuleSet( HB_ChainPosRuleSet*  cprs )
{
  HB_UShort          n, count;

  HB_ChainPosRule*  cpr;


  if ( cprs->ChainPosRule )
  {
    count = cprs->ChainPosRuleCount;
    cpr   = cprs->ChainPosRule;

    for ( n = 0; n < count; n++ )
      Free_ChainPosRule( &cpr[n] );

    FREE( cpr );
  }
}

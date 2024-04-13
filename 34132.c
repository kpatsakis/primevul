static void  Free_ChainContextPos1( HB_ChainContextPosFormat1*  ccpf1 )
{
  HB_UShort             n, count;

  HB_ChainPosRuleSet*  cprs;


  if ( ccpf1->ChainPosRuleSet )
  {
    count = ccpf1->ChainPosRuleSetCount;
    cprs  = ccpf1->ChainPosRuleSet;

    for ( n = 0; n < count; n++ )
      Free_ChainPosRuleSet( &cprs[n] );

    FREE( cprs );
  }

  _HB_OPEN_Free_Coverage( &ccpf1->Coverage );
}

static void  Free_ContextPos1( HB_ContextPosFormat1*  cpf1 )
{
  HB_UShort        n, count;

  HB_PosRuleSet*  prs;


  if ( cpf1->PosRuleSet )
  {
    count = cpf1->PosRuleSetCount;
    prs   = cpf1->PosRuleSet;

    for ( n = 0; n < count; n++ )
      Free_PosRuleSet( &prs[n] );

    FREE( prs );
  }

  _HB_OPEN_Free_Coverage( &cpf1->Coverage );
}

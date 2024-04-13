static void  Free_ChainContextPos( HB_GPOS_SubTable* st )
{
  HB_ChainContextPos*  ccp = &st->chain;

  switch ( ccp->PosFormat )
  {
  case 1:  Free_ChainContextPos1( &ccp->ccpf.ccpf1 ); break;
  case 2:  Free_ChainContextPos2( &ccp->ccpf.ccpf2 ); break;
  case 3:  Free_ChainContextPos3( &ccp->ccpf.ccpf3 ); break;
  default:						      break;
  }
}

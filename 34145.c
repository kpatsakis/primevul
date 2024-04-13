static void  Free_PairPos( HB_GPOS_SubTable* st )
{
  HB_UShort  format1, format2;
  HB_PairPos*     pp = &st->pair;


  format1 = pp->ValueFormat1;
  format2 = pp->ValueFormat2;

  switch ( pp->PosFormat )
  {
  case 1:
    Free_PairPos1( &pp->ppf.ppf1, format1, format2 );
    break;

  case 2:
    Free_PairPos2( &pp->ppf.ppf2, format1, format2 );
    break;

  default:
    break;
  }

  _HB_OPEN_Free_Coverage( &pp->Coverage );
}

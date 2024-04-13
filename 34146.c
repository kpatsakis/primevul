static void  Free_PairPos1( HB_PairPosFormat1*  ppf1,
			    HB_UShort            format1,
			    HB_UShort            format2 )
{
  HB_UShort     n, count;

  HB_PairSet*  ps;


  if ( ppf1->PairSet )
  {
    count = ppf1->PairSetCount;
    ps    = ppf1->PairSet;

    for ( n = 0; n < count; n++ )
      Free_PairSet( &ps[n], format1, format2 );

    FREE( ps );
  }
}

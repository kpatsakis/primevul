static void  Free_PairSet( HB_PairSet*  ps,
			   HB_UShort     format1,
			   HB_UShort     format2 )
{
  HB_UShort             n, count;

  HB_PairValueRecord*  pvr;


  if ( ps->PairValueRecord )
  {
    count = ps->PairValueCount;
    pvr   = ps->PairValueRecord;

    for ( n = 0; n < count; n++ )
    {
      if ( format1 )
	Free_ValueRecord( &pvr[n].Value1, format1 );
      if ( format2 )
	Free_ValueRecord( &pvr[n].Value2, format2 );
    }

    FREE( pvr );
  }
}

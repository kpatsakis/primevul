static void  Free_PairPos2( HB_PairPosFormat2*  ppf2,
			    HB_UShort            format1,
			    HB_UShort            format2)
{
  HB_UShort          m, n, count1, count2;

  HB_Class1Record*  c1r;
  HB_Class2Record*  c2r;


  if ( ppf2->Class1Record )
  {
    c1r    = ppf2->Class1Record;
    count1 = ppf2->Class1Count;
    count2 = ppf2->Class2Count;

    for ( m = 0; m < count1; m++ )
    {
      c2r = c1r[m].Class2Record;

      for ( n = 0; n < count2; n++ )
      {
	if ( format1 )
	  Free_ValueRecord( &c2r[n].Value1, format1 );
	if ( format2 )
	  Free_ValueRecord( &c2r[n].Value2, format2 );
      }

      FREE( c2r );
    }

    FREE( c1r );

    _HB_OPEN_Free_ClassDefinition( &ppf2->ClassDef2 );
    _HB_OPEN_Free_ClassDefinition( &ppf2->ClassDef1 );
  }
}

static void  Free_LigatureAttach( HB_LigatureAttach*  lat,
				  HB_UShort            num_classes )
{
  HB_UShort        m, n, count;

  HB_ComponentRecord*  cr;
  HB_Anchor*           lan;


  if ( lat->ComponentRecord )
  {
    count = lat->ComponentCount;
    cr    = lat->ComponentRecord;

    for ( m = 0; m < count; m++ )
    {
      lan = cr[m].LigatureAnchor;

      for ( n = 0; n < num_classes; n++ )
	Free_Anchor( &lan[n] );

      FREE( lan );
    }

    FREE( cr );
  }
}

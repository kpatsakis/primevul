static void  Free_PosClassSet( HB_PosClassSet*  pcs )
{
  HB_UShort          n, count;

  HB_PosClassRule*  pcr;


  if ( pcs->PosClassRule )
  {
    count = pcs->PosClassRuleCount;
    pcr   = pcs->PosClassRule;

    for ( n = 0; n < count; n++ )
      Free_PosClassRule( &pcr[n] );

    FREE( pcr );
  }
}

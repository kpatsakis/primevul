static void  Free_MarkArray( HB_MarkArray*  ma )
{
  HB_UShort        n, count;

  HB_MarkRecord*  mr;


  if ( ma->MarkRecord )
  {
    count = ma->MarkCount;
    mr    = ma->MarkRecord;

    for ( n = 0; n < count; n++ )
      Free_Anchor( &mr[n].MarkAnchor );

    FREE( mr );
  }
}

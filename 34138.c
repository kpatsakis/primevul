static void  Free_ContextPos2( HB_ContextPosFormat2*  cpf2 )
{
  HB_UShort         n, count;

  HB_PosClassSet*  pcs;


  if ( cpf2->PosClassSet )
  {
    count = cpf2->PosClassSetCount;
    pcs   = cpf2->PosClassSet;

    for ( n = 0; n < count; n++ )
      Free_PosClassSet( &pcs[n] );

    FREE( pcs );
  }

  _HB_OPEN_Free_ClassDefinition( &cpf2->ClassDef );
  _HB_OPEN_Free_Coverage( &cpf2->Coverage );
}

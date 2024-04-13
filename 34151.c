static void  Free_SinglePos( HB_GPOS_SubTable* st )
{
  HB_UShort         n, count, format;
  HB_SinglePos*   sp = &st->single;

  HB_ValueRecord*  v;


  format = sp->ValueFormat;

  switch ( sp->PosFormat )
  {
  case 1:
    Free_ValueRecord( &sp->spf.spf1.Value, format );
    break;

  case 2:
    if ( sp->spf.spf2.Value )
    {
      count = sp->spf.spf2.ValueCount;
      v     = sp->spf.spf2.Value;

      for ( n = 0; n < count; n++ )
	Free_ValueRecord( &v[n], format );

      FREE( v );
    }
    break;
  default:
    break;
  }

  _HB_OPEN_Free_Coverage( &sp->Coverage );
}

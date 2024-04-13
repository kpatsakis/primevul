static void  Free_CursivePos( HB_GPOS_SubTable* st )
{
  HB_UShort             n, count;
  HB_CursivePos*  cp = &st->cursive;

  HB_EntryExitRecord*  eer;


  if ( cp->EntryExitRecord )
  {
    count = cp->EntryExitCount;
    eer   = cp->EntryExitRecord;

    for ( n = 0; n < count; n++ )
    {
      Free_Anchor( &eer[n].EntryAnchor );
      Free_Anchor( &eer[n].ExitAnchor );
    }

    FREE( eer );
  }

  _HB_OPEN_Free_Coverage( &cp->Coverage );
}

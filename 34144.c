static void  Free_MarkMarkPos( HB_GPOS_SubTable* st)
{
  HB_MarkMarkPos* mmp = &st->markmark;

  Free_Mark2Array( &mmp->Mark2Array, mmp->ClassCount );
  Free_MarkArray( &mmp->Mark1Array );
  _HB_OPEN_Free_Coverage( &mmp->Mark2Coverage );
  _HB_OPEN_Free_Coverage( &mmp->Mark1Coverage );
}

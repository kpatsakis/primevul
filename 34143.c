static void  Free_MarkBasePos( HB_GPOS_SubTable* st )
{
  HB_MarkBasePos* mbp = &st->markbase;

  Free_BaseArray( &mbp->BaseArray, mbp->ClassCount );
  Free_MarkArray( &mbp->MarkArray );
  _HB_OPEN_Free_Coverage( &mbp->BaseCoverage );
  _HB_OPEN_Free_Coverage( &mbp->MarkCoverage );
}

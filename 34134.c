static void  Free_ChainPosClassRule( HB_ChainPosClassRule*  cpcr )
{
  FREE( cpcr->PosLookupRecord );
  FREE( cpcr->Lookahead );
  FREE( cpcr->Input );
  FREE( cpcr->Backtrack );
}

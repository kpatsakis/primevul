static void  Free_PosRule( HB_PosRule*  pr )
{
  FREE( pr->PosLookupRecord );
  FREE( pr->Input );
}

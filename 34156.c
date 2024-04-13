HB_Error  HB_Done_GPOS_Table( HB_GPOSHeader* gpos )
{
  _HB_OPEN_Free_LookupList( &gpos->LookupList, HB_Type_GPOS );
  _HB_OPEN_Free_FeatureList( &gpos->FeatureList );
  _HB_OPEN_Free_ScriptList( &gpos->ScriptList );

  FREE( gpos );

  return HB_Err_Ok;
}

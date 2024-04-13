HB_Error  HB_GPOS_Add_Feature( HB_GPOSHeader*  gpos,
			       HB_UShort        feature_index,
			       HB_UInt          property )
{
  HB_UShort    i;

  HB_Feature  feature;
  HB_UInt*     properties;
  HB_UShort*   index;
  HB_UShort    lookup_count;

  /* Each feature can only be added once */

  if ( !gpos ||
       feature_index >= gpos->FeatureList.FeatureCount ||
       gpos->FeatureList.ApplyCount == gpos->FeatureList.FeatureCount )
    return ERR(HB_Err_Invalid_Argument);

  gpos->FeatureList.ApplyOrder[gpos->FeatureList.ApplyCount++] = feature_index;

  properties = gpos->LookupList.Properties;

  feature = gpos->FeatureList.FeatureRecord[feature_index].Feature;
  index   = feature.LookupListIndex;
  lookup_count = gpos->LookupList.LookupCount;

  for ( i = 0; i < feature.LookupListCount; i++ )
  {
    HB_UShort lookup_index = index[i];
    if (lookup_index < lookup_count)
      properties[lookup_index] |= property;
  }

  return HB_Err_Ok;
}

HB_Error  HB_GPOS_Apply_String( HB_Font            font,
				HB_GPOSHeader*    gpos,
				HB_UShort          load_flags,
				HB_Buffer         buffer,
				HB_Bool            dvi,
				HB_Bool            r2l )
{
  HB_Error       error, retError = HB_Err_Not_Covered;
  GPOS_Instance  gpi;
  int            i, j, lookup_count, num_features;

  if ( !font || !gpos || !buffer )
    return ERR(HB_Err_Invalid_Argument);

  if ( buffer->in_length == 0 )
    return HB_Err_Not_Covered;

  gpi.font       = font;
  gpi.gpos       = gpos;
  gpi.load_flags = load_flags;
  gpi.r2l        = r2l;
  gpi.dvi        = dvi;

  lookup_count = gpos->LookupList.LookupCount;
  num_features = gpos->FeatureList.ApplyCount;

  if ( num_features )
    {
      error = _hb_buffer_clear_positions( buffer );
      if ( error )
	return error;
    }

  for ( i = 0; i < num_features; i++ )
  {
    HB_UShort  feature_index = gpos->FeatureList.ApplyOrder[i];
    HB_Feature feature = gpos->FeatureList.FeatureRecord[feature_index].Feature;

    for ( j = 0; j < feature.LookupListCount; j++ )
    {
      HB_UShort lookup_index = feature.LookupListIndex[j];

      /* Skip nonexistant lookups */
      if (lookup_index >= lookup_count)
       continue;

      error = GPOS_Do_String_Lookup( &gpi, lookup_index, buffer );
      if ( error )
      {
	if ( error != HB_Err_Not_Covered )
	  return error;
      }
      else
	retError = error;
    }
  }

  if ( num_features )
    {
  error = Position_CursiveChain ( buffer );
  if ( error )
    return error;
    }

  return retError;
}

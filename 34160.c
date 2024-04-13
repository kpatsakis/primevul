HB_Error  HB_GPOS_Query_Scripts( HB_GPOSHeader*  gpos,
				 HB_UInt**       script_tag_list )
{
  HB_Error           error;
  HB_UShort          n;
  HB_UInt*          stl;

  HB_ScriptList*    sl;
  HB_ScriptRecord*  sr;


  if ( !gpos || !script_tag_list )
    return ERR(HB_Err_Invalid_Argument);

  sl = &gpos->ScriptList;
  sr = sl->ScriptRecord;

  if ( ALLOC_ARRAY( stl, sl->ScriptCount + 1, HB_UInt ) )
    return error;

  for ( n = 0; n < sl->ScriptCount; n++ )
    stl[n] = sr[n].ScriptTag;
  stl[n] = 0;

  *script_tag_list = stl;

  return HB_Err_Ok;
}

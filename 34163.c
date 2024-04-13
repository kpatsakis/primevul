HB_Error  HB_GPOS_Select_Script( HB_GPOSHeader*  gpos,
				 HB_UInt         script_tag,
				 HB_UShort*       script_index )
{
  HB_UShort          n;

  HB_ScriptList*    sl;
  HB_ScriptRecord*  sr;


  if ( !gpos || !script_index )
    return ERR(HB_Err_Invalid_Argument);

  sl = &gpos->ScriptList;
  sr = sl->ScriptRecord;

  for ( n = 0; n < sl->ScriptCount; n++ )
    if ( script_tag == sr[n].ScriptTag )
    {
      *script_index = n;

      return HB_Err_Ok;
    }

  return HB_Err_Not_Covered;
}

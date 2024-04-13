HB_Error  HB_GPOS_Query_Languages( HB_GPOSHeader*  gpos,
				   HB_UShort        script_index,
				   HB_UInt**       language_tag_list )
{
  HB_Error            error;
  HB_UShort           n;
  HB_UInt*           ltl;

  HB_ScriptList*     sl;
  HB_ScriptRecord*   sr;
  HB_ScriptTable*    s;
  HB_LangSysRecord*  lsr;


  if ( !gpos || !language_tag_list )
    return ERR(HB_Err_Invalid_Argument);

  sl = &gpos->ScriptList;
  sr = sl->ScriptRecord;

  if ( script_index >= sl->ScriptCount )
    return ERR(HB_Err_Invalid_Argument);

  s   = &sr[script_index].Script;
  lsr = s->LangSysRecord;

  if ( ALLOC_ARRAY( ltl, s->LangSysCount + 1, HB_UInt ) )
    return error;

  for ( n = 0; n < s->LangSysCount; n++ )
    ltl[n] = lsr[n].LangSysTag;
  ltl[n] = 0;

  *language_tag_list = ltl;

  return HB_Err_Ok;
}

HB_Error  HB_GPOS_Select_Feature( HB_GPOSHeader*  gpos,
				  HB_UInt         feature_tag,
				  HB_UShort        script_index,
				  HB_UShort        language_index,
				  HB_UShort*       feature_index )
{
  HB_UShort           n;

  HB_ScriptList*     sl;
  HB_ScriptRecord*   sr;
  HB_ScriptTable*         s;
  HB_LangSysRecord*  lsr;
  HB_LangSys*        ls;
  HB_UShort*          fi;

  HB_FeatureList*    fl;
  HB_FeatureRecord*  fr;


  if ( !gpos || !feature_index )
    return ERR(HB_Err_Invalid_Argument);

  sl = &gpos->ScriptList;
  sr = sl->ScriptRecord;

  fl = &gpos->FeatureList;
  fr = fl->FeatureRecord;

  if ( script_index >= sl->ScriptCount )
    return ERR(HB_Err_Invalid_Argument);

  s   = &sr[script_index].Script;
  lsr = s->LangSysRecord;

  if ( language_index == 0xFFFF )
    ls = &s->DefaultLangSys;
  else
  {
    if ( language_index >= s->LangSysCount )
      return ERR(HB_Err_Invalid_Argument);

    ls = &lsr[language_index].LangSys;
  }

  fi = ls->FeatureIndex;

  for ( n = 0; n < ls->FeatureCount; n++ )
  {
    if ( fi[n] >= fl->FeatureCount )
      return ERR(HB_Err_Invalid_SubTable_Format);

    if ( feature_tag == fr[fi[n]].FeatureTag )
    {
      *feature_index = fi[n];

      return HB_Err_Ok;
    }
  }

  return HB_Err_Not_Covered;
}

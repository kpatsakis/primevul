HB_Error  HB_GPOS_Register_MM_Function( HB_GPOSHeader*  gpos,
					HB_MMFunction   mmfunc,
					void*            data )
{
  if ( !gpos )
    return ERR(HB_Err_Invalid_Argument);

  gpos->mmfunc = mmfunc;
  gpos->data   = data;

  return HB_Err_Ok;
}

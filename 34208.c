static HB_Error  default_mmfunc( HB_Font      font,
				 HB_UShort    metric_id,
				 HB_Fixed*      metric_value,
				 void*        data )
{
  HB_UNUSED(font);
  HB_UNUSED(metric_id);
  HB_UNUSED(metric_value);
  HB_UNUSED(data);
  return ERR(HB_Err_Not_Covered); /* ERR() call intended */
}

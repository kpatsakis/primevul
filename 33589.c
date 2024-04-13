gpgsm_set_colon_line_handler (void *engine, engine_colon_line_handler_t fnc,
			      void *fnc_value)
{
  engine_gpgsm_t gpgsm = engine;

  gpgsm->colon.fnc = fnc;
  gpgsm->colon.fnc_value = fnc_value;
  gpgsm->colon.any = 0;
  return 0;
}

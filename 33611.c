uiserver_set_colon_line_handler (void *engine, engine_colon_line_handler_t fnc,
			      void *fnc_value)
{
  engine_uiserver_t uiserver = engine;

  uiserver->colon.fnc = fnc;
  uiserver->colon.fnc_value = fnc_value;
  uiserver->colon.any = 0;
  return 0;
}

uiserver_set_status_handler (void *engine, engine_status_handler_t fnc,
			  void *fnc_value)
{
  engine_uiserver_t uiserver = engine;

  uiserver->status.fnc = fnc;
  uiserver->status.fnc_value = fnc_value;
}

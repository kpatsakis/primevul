static PHP_GINIT_FUNCTION(ps) /* {{{ */
{
	int i;

	ps_globals->save_path = NULL;
	ps_globals->session_name = NULL;
	ps_globals->id = NULL;
	ps_globals->mod = NULL;
	ps_globals->serializer = NULL;
	ps_globals->mod_data = NULL;
	ps_globals->session_status = php_session_none;
	ps_globals->default_mod = NULL;
	ps_globals->mod_user_implemented = 0;
	ps_globals->mod_user_is_open = 0;
	for (i = 0; i < 7; i++) {
		ps_globals->mod_user_names.names[i] = NULL;
	}
	ps_globals->http_session_vars = NULL;
}
/* }}} */

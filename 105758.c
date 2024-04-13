MYSQLND_METHOD(mysqlnd_conn_data, free_options)(MYSQLND_CONN_DATA * conn TSRMLS_DC)
{
	zend_bool pers = conn->persistent;

	if (conn->options->charset_name) {
		mnd_pefree(conn->options->charset_name, pers);
		conn->options->charset_name = NULL;
	}
	if (conn->options->auth_protocol) {
		mnd_pefree(conn->options->auth_protocol, pers);
		conn->options->auth_protocol = NULL;
	}
	if (conn->options->num_commands) {
		unsigned int i;
		for (i = 0; i < conn->options->num_commands; i++) {
			/* allocated with pestrdup */
			mnd_pefree(conn->options->init_commands[i], pers);
		}
		mnd_pefree(conn->options->init_commands, pers);
		conn->options->init_commands = NULL;
	}
	if (conn->options->cfg_file) {
		mnd_pefree(conn->options->cfg_file, pers);
		conn->options->cfg_file = NULL;
	}
	if (conn->options->cfg_section) {
		mnd_pefree(conn->options->cfg_section, pers);
		conn->options->cfg_section = NULL;
	}
}

MYSQLND_METHOD(mysqlnd_conn_data, free_contents)(MYSQLND_CONN_DATA * conn TSRMLS_DC)
{
	zend_bool pers = conn->persistent;

	DBG_ENTER("mysqlnd_conn_data::free_contents");

	mysqlnd_local_infile_default(conn);
	if (conn->current_result) {
		conn->current_result->m.free_result(conn->current_result, TRUE TSRMLS_CC);
		conn->current_result = NULL;
	}

	if (conn->net) {
		conn->net->m.free_contents(conn->net TSRMLS_CC);
	}

	DBG_INF("Freeing memory of members");

	if (conn->host) {
		mnd_pefree(conn->host, pers);
		conn->host = NULL;
	}
	if (conn->user) {
		mnd_pefree(conn->user, pers);
		conn->user = NULL;
	}
	if (conn->passwd) {
		mnd_pefree(conn->passwd, pers);
		conn->passwd = NULL;
	}
	if (conn->connect_or_select_db) {
		mnd_pefree(conn->connect_or_select_db, pers);
		conn->connect_or_select_db = NULL;
	}
	if (conn->unix_socket) {
		mnd_pefree(conn->unix_socket, pers);
		conn->unix_socket = NULL;
	}
	DBG_INF_FMT("scheme=%s", conn->scheme);
	if (conn->scheme) {
		mnd_pefree(conn->scheme, pers);
		conn->scheme = NULL;
	}
	if (conn->server_version) {
		mnd_pefree(conn->server_version, pers);
		conn->server_version = NULL;
	}
	if (conn->host_info) {
		mnd_pefree(conn->host_info, pers);
		conn->host_info = NULL;
	}
	if (conn->auth_plugin_data) {
		mnd_pefree(conn->auth_plugin_data, pers);
		conn->auth_plugin_data = NULL;
	}
	if (conn->last_message) {
		mnd_pefree(conn->last_message, pers);
		conn->last_message = NULL;
	}
	if (conn->error_info->error_list) {
		zend_llist_clean(conn->error_info->error_list);
		mnd_pefree(conn->error_info->error_list, pers);
		conn->error_info->error_list = NULL;
	}
	conn->charset = NULL;
	conn->greet_charset = NULL;

	DBG_VOID_RETURN;
}

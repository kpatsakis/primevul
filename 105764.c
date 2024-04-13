MYSQLND_METHOD(mysqlnd_conn, connect)(MYSQLND * conn_handle,
						 const char * host, const char * user,
						 const char * passwd, unsigned int passwd_len,
						 const char * db, unsigned int db_len,
						 unsigned int port,
						 const char * socket_or_pipe,
						 unsigned int mysql_flags
						 TSRMLS_DC)
{
	size_t this_func = STRUCT_OFFSET(struct st_mysqlnd_conn_data_methods, connect);
	enum_func_status ret = FAIL;
	MYSQLND_CONN_DATA * conn = conn_handle->data;

	DBG_ENTER("mysqlnd_conn::connect");

	if (PASS == conn->m->local_tx_start(conn, this_func TSRMLS_CC)) {
		ret = conn->m->connect(conn, host, user, passwd, passwd_len, db, db_len, port, socket_or_pipe, mysql_flags TSRMLS_CC);

		conn->m->local_tx_end(conn, this_func, FAIL TSRMLS_CC);
	}
	DBG_RETURN(ret);
}

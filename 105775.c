PHPAPI MYSQLND * mysqlnd_connect(MYSQLND * conn_handle,
						 const char * host, const char * user,
						 const char * passwd, unsigned int passwd_len,
						 const char * db, unsigned int db_len,
						 unsigned int port,
						 const char * socket_or_pipe,
						 unsigned int mysql_flags
						 TSRMLS_DC)
{
	enum_func_status ret = FAIL;
	zend_bool self_alloced = FALSE;

	DBG_ENTER("mysqlnd_connect");
	DBG_INF_FMT("host=%s user=%s db=%s port=%u flags=%u", host?host:"", user?user:"", db?db:"", port, mysql_flags);

	if (!conn_handle) {
		self_alloced = TRUE;
		if (!(conn_handle = mysqlnd_init(FALSE))) {
			/* OOM */
			DBG_RETURN(NULL);
		}
	}

	ret = conn_handle->m->connect(conn_handle, host, user, passwd, passwd_len, db, db_len, port, socket_or_pipe, mysql_flags TSRMLS_CC);

	if (ret == FAIL) {
		if (self_alloced) {
			/*
			  We have alloced, thus there are no references to this
			  object - we are free to kill it!
			*/
			conn_handle->m->dtor(conn_handle TSRMLS_CC);
		}
		DBG_RETURN(NULL);
	}
	DBG_RETURN(conn_handle);
}

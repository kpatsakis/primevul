MYSQLND_METHOD(mysqlnd_conn_data, sqlstate)(const MYSQLND_CONN_DATA * const conn TSRMLS_DC)
{
	return conn->error_info->sqlstate[0] ? conn->error_info->sqlstate:MYSQLND_SQLSTATE_NULL;
}

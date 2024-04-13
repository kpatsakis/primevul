MYSQLND_METHOD(mysqlnd_conn_data, errno)(const MYSQLND_CONN_DATA * const conn TSRMLS_DC)
{
	return conn->error_info->error_no;
}

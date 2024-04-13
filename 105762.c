MYSQLND_METHOD(mysqlnd_conn_data, restart_psession)(MYSQLND_CONN_DATA * conn TSRMLS_DC)
{
	DBG_ENTER("mysqlnd_conn_data::restart_psession");
	MYSQLND_INC_CONN_STATISTIC(conn->stats, STAT_CONNECT_REUSED);
	/* Free here what should not be seen by the next script */
	if (conn->last_message) {
		mnd_pefree(conn->last_message, conn->persistent);
		conn->last_message = NULL;
	}
	DBG_RETURN(PASS);
}

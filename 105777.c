MYSQLND ** mysqlnd_stream_array_check_for_readiness(MYSQLND ** conn_array TSRMLS_DC)
{
	int cnt = 0;
	MYSQLND **p = conn_array, **p_p;
	MYSQLND **ret = NULL;

	while (*p) {
		if (CONN_GET_STATE((*p)->data) <= CONN_READY || CONN_GET_STATE((*p)->data) == CONN_QUIT_SENT) {
			cnt++;
		}
		p++;
	}
	if (cnt) {
		MYSQLND **ret_p = ret = ecalloc(cnt + 1, sizeof(MYSQLND *));
		p_p = p = conn_array;
		while (*p) {
			if (CONN_GET_STATE((*p)->data) <= CONN_READY || CONN_GET_STATE((*p)->data) == CONN_QUIT_SENT) {
				*ret_p = *p;
				*p = NULL;
				ret_p++;
			} else {
				*p_p = *p;
				p_p++;
			}
			p++;
		}
		*ret_p = NULL;
	}
	return ret;
}

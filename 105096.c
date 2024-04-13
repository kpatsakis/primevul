static zend_bool early_find_sid_in(zval *dest, int where, php_session_rfc1867_progress *progress TSRMLS_DC) /* {{{ */
{
	zval **ppid;

	if (!PG(http_globals)[where]) {
		return 0;
	}

	if (zend_hash_find(Z_ARRVAL_P(PG(http_globals)[where]), PS(session_name), progress->sname_len+1, (void **)&ppid) == SUCCESS
			&& Z_TYPE_PP(ppid) == IS_STRING) {
		zval_dtor(dest);
		ZVAL_ZVAL(dest, *ppid, 1, 0);
		return 1;
	}

	return 0;
} /* }}} */

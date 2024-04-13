PHP_FUNCTION(pg_send_execute)
{
	zval *pgsql_link;
	zval *pv_param_arr, **tmp;
	int num_params = 0;
	char **params = NULL;
	char *stmtname;
	int stmtname_len, id = -1;
	PGconn *pgsql;
	PGresult *res;
	int leftover = 0;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsa", &pgsql_link, &stmtname, &stmtname_len, &pv_param_arr) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 1)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}
	while ((res = PQgetResult(pgsql))) {
		PQclear(res);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "There are results on this connection. Call pg_get_result() until it returns FALSE");
	}

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(pv_param_arr));
	num_params = zend_hash_num_elements(Z_ARRVAL_P(pv_param_arr));
	if (num_params > 0) {
		int i = 0;
		params = (char **)safe_emalloc(sizeof(char *), num_params, 0);
		
		for(i = 0; i < num_params; i++) {
			if (zend_hash_get_current_data(Z_ARRVAL_P(pv_param_arr), (void **) &tmp) == FAILURE) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING,"Error getting parameter");
				_php_pgsql_free_params(params, num_params);
				RETURN_FALSE;
			}

			if (Z_TYPE_PP(tmp) == IS_NULL) {
				params[i] = NULL;
			} else {
				zval tmp_val = **tmp;
				zval_copy_ctor(&tmp_val);
				convert_to_string(&tmp_val);
				if (Z_TYPE(tmp_val) != IS_STRING) {
					php_error_docref(NULL TSRMLS_CC, E_WARNING,"Error converting parameter");
					zval_dtor(&tmp_val);
					_php_pgsql_free_params(params, num_params);
					RETURN_FALSE;
				}
				params[i] = estrndup(Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
				zval_dtor(&tmp_val);
			}

			zend_hash_move_forward(Z_ARRVAL_P(pv_param_arr));
		}
	}

	if (!PQsendQueryPrepared(pgsql, stmtname, num_params, (const char * const *)params, NULL, NULL, 0)) {
		if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
			PQreset(pgsql);
		}
		if (!PQsendQueryPrepared(pgsql, stmtname, num_params, (const char * const *)params, NULL, NULL, 0)) {
			_php_pgsql_free_params(params, num_params);
			RETURN_FALSE;
		}
	}
	_php_pgsql_free_params(params, num_params);
	/* Wait to finish sending buffer */
	while ((ret = PQflush(pgsql))) {
		if (ret == -1) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Could not empty postgres send buffer");
			break;
		}
		usleep(10000);
	}
	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot set connection to blocking mode");
	}
	RETURN_TRUE;
}

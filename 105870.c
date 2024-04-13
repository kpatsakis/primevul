PHP_FUNCTION(pg_query_params)
{
	zval *pgsql_link = NULL;
	zval *pv_param_arr, **tmp;
	char *query;
	int query_len, id = -1, argc = ZEND_NUM_ARGS();
	int leftover = 0;
	int num_params = 0;
	char **params = NULL;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	pgsql_result_handle *pg_result;
	
	if (argc == 2) {
		if (zend_parse_parameters(argc TSRMLS_CC, "sa", &query, &query_len, &pv_param_arr) == FAILURE) {
			return;
		}
		id = PGG(default_link);
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc TSRMLS_CC, "rsa", &pgsql_link, &query, &query_len, &pv_param_arr) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE,"Cannot set connection to blocking mode");
		RETURN_FALSE;
	}
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Found results on this connection. Use pg_get_result() to get these results first");
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
				convert_to_cstring(&tmp_val);
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

	pgsql_result = PQexecParams(pgsql, query, num_params, 
					NULL, (const char * const *)params, NULL, NULL, 0);
	if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
		PQclear(pgsql_result);
		PQreset(pgsql);
		pgsql_result = PQexecParams(pgsql, query, num_params, 
						NULL, (const char * const *)params, NULL, NULL, 0);
	}

	if (pgsql_result) {
		status = PQresultStatus(pgsql_result);
	} else {
		status = (ExecStatusType) PQstatus(pgsql);
	}
	
	_php_pgsql_free_params(params, num_params);

	switch (status) {
		case PGRES_EMPTY_QUERY:
		case PGRES_BAD_RESPONSE:
		case PGRES_NONFATAL_ERROR:
		case PGRES_FATAL_ERROR:
			PHP_PQ_ERROR("Query failed: %s", pgsql);
			PQclear(pgsql_result);
			RETURN_FALSE;
			break;
		case PGRES_COMMAND_OK: /* successful command that did not return rows */
		default:
			if (pgsql_result) {
				pg_result = (pgsql_result_handle *) emalloc(sizeof(pgsql_result_handle));
				pg_result->conn = pgsql;
				pg_result->result = pgsql_result;
				pg_result->row = 0;
				ZEND_REGISTER_RESOURCE(return_value, pg_result, le_result);
			} else {
				PQclear(pgsql_result);
				RETURN_FALSE;
			}
			break;
	}
}

static void php_pgsql_do_connect(INTERNAL_FUNCTION_PARAMETERS, int persistent)
{
	char *host=NULL,*port=NULL,*options=NULL,*tty=NULL,*dbname=NULL,*connstring=NULL;
	PGconn *pgsql;
	smart_str str = {0};
	zval *args;
	uint32_t i;
	int connect_type = 0;
	PGresult *pg_result;

	args = (zval *)safe_emalloc(ZEND_NUM_ARGS(), sizeof(zval), 0);
	if (ZEND_NUM_ARGS() < 1 || ZEND_NUM_ARGS() > 5
			|| zend_get_parameters_array_ex(ZEND_NUM_ARGS(), args) == FAILURE) {
		efree(args);
		WRONG_PARAM_COUNT;
	}

	smart_str_appends(&str, "pgsql");
	
	for (i = 0; i < ZEND_NUM_ARGS(); i++) {
		/* make sure that the PGSQL_CONNECT_FORCE_NEW bit is not part of the hash so that subsequent connections
		 * can re-use this connection. Bug #39979
		 */ 
		if (i == 1 && ZEND_NUM_ARGS() == 2 && Z_TYPE(args[i]) == IS_LONG) {
			if (Z_LVAL(args[1]) == PGSQL_CONNECT_FORCE_NEW) {
				continue;
			} else if (Z_LVAL(args[1]) & PGSQL_CONNECT_FORCE_NEW) {
				smart_str_append_long(&str, Z_LVAL(args[1]) ^ PGSQL_CONNECT_FORCE_NEW);
			}
		}
		convert_to_string_ex(&args[i]);
		smart_str_appendc(&str, '_');
		smart_str_appendl(&str, Z_STRVAL(args[i]), Z_STRLEN(args[i]));
	}

	smart_str_0(&str);

	if (ZEND_NUM_ARGS() == 1) { /* new style, using connection string */
		connstring = Z_STRVAL(args[0]);
	} else if (ZEND_NUM_ARGS() == 2 ) { /* Safe to add conntype_option, since 2 args was illegal */
		connstring = Z_STRVAL(args[0]);
		convert_to_long_ex(&args[1]);
		connect_type = (int)Z_LVAL(args[1]);
	} else {
		host = Z_STRVAL(args[0]);
		port = Z_STRVAL(args[1]);
		dbname = Z_STRVAL(args[ZEND_NUM_ARGS()-1]);

		switch (ZEND_NUM_ARGS()) {
		case 5:
			tty = Z_STRVAL(args[3]);
			/* fall through */
		case 4:
			options = Z_STRVAL(args[2]);
			break;
		}
	}
	efree(args);

	if (persistent && PGG(allow_persistent)) {
		zend_resource *le;
		
		/* try to find if we already have this link in our persistent list */
		if ((le = zend_hash_find_ptr(&EG(persistent_list), str.s)) == NULL) {  /* we don't */
			zend_resource new_le;
			
			if (PGG(max_links) != -1 && PGG(num_links) >= PGG(max_links)) {
				php_error_docref(NULL, E_WARNING,
								 "Cannot create new link. Too many open links (%pd)", PGG(num_links));
				goto err;
			}
			if (PGG(max_persistent) != -1 && PGG(num_persistent) >= PGG(max_persistent)) {
				php_error_docref(NULL, E_WARNING,
								 "Cannot create new link. Too many open persistent links (%pd)", PGG(num_persistent));
				goto err;
			}

			/* create the link */
			if (connstring) {
				pgsql = PQconnectdb(connstring);
			} else {
				pgsql = PQsetdb(host, port, options, tty, dbname);
			}
			if (pgsql == NULL || PQstatus(pgsql) == CONNECTION_BAD) {
				PHP_PQ_ERROR("Unable to connect to PostgreSQL server: %s", pgsql)
				if (pgsql) {
					PQfinish(pgsql);
				}
				goto err;
			}

			/* hash it up */
			new_le.type = le_plink;
			new_le.ptr = pgsql;
			if (zend_hash_str_update_mem(&EG(persistent_list), str.s->val, str.s->len, &new_le, sizeof(zend_resource)) == NULL) {
				goto err;
			}
			PGG(num_links)++;
			PGG(num_persistent)++;
		} else {  /* we do */
			if (le->type != le_plink) {
				RETURN_FALSE;
			}
			/* ensure that the link did not die */
			if (PGG(auto_reset_persistent) & 1) {
				/* need to send & get something from backend to
				   make sure we catch CONNECTION_BAD every time */
				PGresult *pg_result;
				pg_result = PQexec(le->ptr, "select 1");
				PQclear(pg_result);
			}
			if (PQstatus(le->ptr) == CONNECTION_BAD) { /* the link died */
				if (le->ptr == NULL) {
					if (connstring) {
						le->ptr = PQconnectdb(connstring);
					} else {
						le->ptr = PQsetdb(host,port,options,tty,dbname);
					}
				}
				else {
					PQreset(le->ptr);
				}
				if (le->ptr == NULL || PQstatus(le->ptr) == CONNECTION_BAD) {
					php_error_docref(NULL, E_WARNING,"PostgreSQL link lost, unable to reconnect");
					zend_hash_del(&EG(persistent_list), str.s);
					goto err;
				}
			}
			pgsql = (PGconn *) le->ptr;
#if HAVE_PQPROTOCOLVERSION && HAVE_PQPARAMETERSTATUS
			if (PQprotocolVersion(pgsql) >= 3 && atof(PQparameterStatus(pgsql, "server_version")) >= 7.2) {
#else
			if (atof(PG_VERSION) >= 7.2) {
#endif
				pg_result = PQexec(pgsql, "RESET ALL;");
				PQclear(pg_result);
			}
		}
		ZEND_REGISTER_RESOURCE(return_value, pgsql, le_plink);
	} else { /* Non persistent connection */
		zend_resource *index_ptr, new_index_ptr;

		/* first we check the hash for the hashed_details key.  if it exists,
		 * it should point us to the right offset where the actual pgsql link sits.
		 * if it doesn't, open a new pgsql link, add it to the resource list,
		 * and add a pointer to it with hashed_details as the key.
		 */
		if (!(connect_type & PGSQL_CONNECT_FORCE_NEW)
			&& (index_ptr = zend_hash_find_ptr(&EG(regular_list), str.s)) != NULL) {
			zend_resource *link;

			if (index_ptr->type != le_index_ptr) {
				RETURN_FALSE;
			}

			link = (zend_resource *)index_ptr->ptr;
			if (link->ptr && (link->type == le_link || link->type == le_plink)) {
				php_pgsql_set_default_link(link);
				GC_REFCOUNT(link)++;
				RETVAL_RES(link);
				goto cleanup;
			} else {
				zend_hash_del(&EG(regular_list), str.s);
			}
		}
		if (PGG(max_links) != -1 && PGG(num_links) >= PGG(max_links)) {
			php_error_docref(NULL, E_WARNING, "Cannot create new link. Too many open links (%pd)", PGG(num_links));
			goto err;
		}

		/* Non-blocking connect */
		if (connect_type & PGSQL_CONNECT_ASYNC) {
			if (connstring) {
				pgsql = PQconnectStart(connstring);
				if (pgsql==NULL || PQstatus(pgsql)==CONNECTION_BAD) {
					PHP_PQ_ERROR("Unable to connect to PostgreSQL server: %s", pgsql);
					if (pgsql) {
						PQfinish(pgsql);
					}
					goto err;
				}
			} else {
				php_error_docref(NULL, E_WARNING, "Connection string required for async connections");
				goto err;
			}
		} else {
			if (connstring) {
				pgsql = PQconnectdb(connstring);
			} else {
				pgsql = PQsetdb(host,port,options,tty,dbname);
			}
			if (pgsql==NULL || PQstatus(pgsql)==CONNECTION_BAD) {
				PHP_PQ_ERROR("Unable to connect to PostgreSQL server: %s", pgsql);
				if (pgsql) {
					PQfinish(pgsql);
				}
				goto err;
			}
		}

		/* add it to the list */
		ZEND_REGISTER_RESOURCE(return_value, pgsql, le_link);

		/* add it to the hash */
		new_index_ptr.ptr = (void *) Z_RES_P(return_value);
		new_index_ptr.type = le_index_ptr;
		if (zend_hash_update_mem(&EG(regular_list), str.s, (void *) &new_index_ptr, sizeof(zend_resource)) == NULL) {
			goto err;
		}
		PGG(num_links)++;
	}
	/* set notice processor */
	if (! PGG(ignore_notices) && Z_TYPE_P(return_value) == IS_RESOURCE) {
		PQsetNoticeProcessor(pgsql, _php_pgsql_notice_handler, (void*)Z_RES_HANDLE_P(return_value));
	}
	php_pgsql_set_default_link(Z_RES_P(return_value));
	
cleanup:
	smart_str_free(&str);
	return;

err:
	smart_str_free(&str);
	RETURN_FALSE;
}
/* }}} */

#if 0
/* {{{ php_pgsql_get_default_link
 */
static int php_pgsql_get_default_link(INTERNAL_FUNCTION_PARAMETERS)
{
	if (PGG(default_link)==-1) { /* no link opened yet, implicitly open one */
		ht = 0;
		php_pgsql_do_connect(INTERNAL_FUNCTION_PARAM_PASSTHRU,0);
	}
	return PGG(default_link);
}
/* }}} */
#endif

/* {{{ proto resource pg_connect(string connection_string[, int connect_type] | [string host, string port [, string options [, string tty,]]] string database)
   Open a PostgreSQL connection */
PHP_FUNCTION(pg_connect)
{
	php_pgsql_do_connect(INTERNAL_FUNCTION_PARAM_PASSTHRU,0);
}
/* }}} */

/* {{{ proto resource pg_connect_poll(resource connection)
   Poll the status of an in-progress async PostgreSQL connection attempt*/
PHP_FUNCTION(pg_connect_poll)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	ret = PQconnectPoll(pgsql);

	RETURN_LONG(ret);
}
/* }}} */

/* {{{ proto resource pg_pconnect(string connection_string | [string host, string port [, string options [, string tty,]]] string database)
   Open a persistent PostgreSQL connection */
PHP_FUNCTION(pg_pconnect)
{
	php_pgsql_do_connect(INTERNAL_FUNCTION_PARAM_PASSTHRU,1);
}
/* }}} */

/* {{{ proto bool pg_close([resource connection])
   Close a PostgreSQL connection */ 
PHP_FUNCTION(pg_close)
{
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;

	if (zend_parse_parameters(argc, "|r", &pgsql_link) == FAILURE) {
		return;
	}

	if (argc == 0) {
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (id==-1) { /* explicit resource number */
		zend_list_close(Z_RES_P(pgsql_link));
	}

	if (id!=-1
		|| (pgsql_link && Z_RES_P(pgsql_link) == PGG(default_link))) {
		zend_list_close(PGG(default_link));
		PGG(default_link) = NULL;
	}

	RETURN_TRUE;
}
/* }}} */

#define PHP_PG_DBNAME 1
#define PHP_PG_ERROR_MESSAGE 2
#define PHP_PG_OPTIONS 3
#define PHP_PG_PORT 4
#define PHP_PG_TTY 5
#define PHP_PG_HOST 6
#define PHP_PG_VERSION 7

/* {{{ php_pgsql_get_link_info
 */
static void php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;
	char *msgbuf;
	char *result;

	if (zend_parse_parameters(argc, "|r", &pgsql_link) == FAILURE) {
		return;
	}
	
	if (argc == 0) {
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	switch(entry_type) {
		case PHP_PG_DBNAME:
			result = PQdb(pgsql);
			break;
		case PHP_PG_ERROR_MESSAGE:
			result = PQErrorMessageTrim(pgsql, &msgbuf);
			RETVAL_STRING(result);
			efree(result);
			return;
		case PHP_PG_OPTIONS:
			result = PQoptions(pgsql);
			break;
		case PHP_PG_PORT:
			result = PQport(pgsql);
			break;
		case PHP_PG_TTY:
			result = PQtty(pgsql);
			break;
		case PHP_PG_HOST:
			result = PQhost(pgsql);
			break;
		case PHP_PG_VERSION:
			array_init(return_value);
			add_assoc_string(return_value, "client", PG_VERSION);
#if HAVE_PQPROTOCOLVERSION
			add_assoc_long(return_value, "protocol", PQprotocolVersion(pgsql));
#if HAVE_PQPARAMETERSTATUS
			if (PQprotocolVersion(pgsql) >= 3) {
				/* 8.0 or grater supports protorol version 3 */
				char *tmp;
				add_assoc_string(return_value, "server", (char*)PQparameterStatus(pgsql, "server_version"));
				tmp = (char*)PQparameterStatus(pgsql, "server_encoding");
				add_assoc_string(return_value, "server_encoding", tmp);
				tmp = (char*)PQparameterStatus(pgsql, "client_encoding");
				add_assoc_string(return_value, "client_encoding", tmp);
				tmp = (char*)PQparameterStatus(pgsql, "is_superuser");
				add_assoc_string(return_value, "is_superuser", tmp);
				tmp = (char*)PQparameterStatus(pgsql, "session_authorization");
				add_assoc_string(return_value, "session_authorization", tmp);
				tmp = (char*)PQparameterStatus(pgsql, "DateStyle");
				add_assoc_string(return_value, "DateStyle", tmp);
				tmp = (char*)PQparameterStatus(pgsql, "IntervalStyle");
				add_assoc_string(return_value, "IntervalStyle", tmp ? tmp : "");
				tmp = (char*)PQparameterStatus(pgsql, "TimeZone");
				add_assoc_string(return_value, "TimeZone", tmp ? tmp : "");
				tmp = (char*)PQparameterStatus(pgsql, "integer_datetimes");
				add_assoc_string(return_value, "integer_datetimes", tmp ? tmp : "");
				tmp = (char*)PQparameterStatus(pgsql, "standard_conforming_strings");
				add_assoc_string(return_value, "standard_conforming_strings", tmp ? tmp : "");
				tmp = (char*)PQparameterStatus(pgsql, "application_name");
				add_assoc_string(return_value, "application_name", tmp ? tmp : "");
			}
#endif
#endif
			return;
		default:
			RETURN_FALSE;
	}
	if (result) {
		RETURN_STRING(result);
	} else {
		RETURN_EMPTY_STRING();
	}
}
/* }}} */

/* {{{ proto string pg_dbname([resource connection])
   Get the database name */ 
PHP_FUNCTION(pg_dbname)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_DBNAME);
}
/* }}} */

/* {{{ proto string pg_last_error([resource connection])
   Get the error message string */
PHP_FUNCTION(pg_last_error)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_ERROR_MESSAGE);
}
/* }}} */

/* {{{ proto string pg_options([resource connection])
   Get the options associated with the connection */
PHP_FUNCTION(pg_options)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_OPTIONS);
}
/* }}} */

/* {{{ proto int pg_port([resource connection])
   Return the port number associated with the connection */
PHP_FUNCTION(pg_port)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_PORT);
}
/* }}} */

/* {{{ proto string pg_tty([resource connection])
   Return the tty name associated with the connection */
PHP_FUNCTION(pg_tty)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_TTY);
}
/* }}} */

/* {{{ proto string pg_host([resource connection])
   Returns the host name associated with the connection */
PHP_FUNCTION(pg_host)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_HOST);
}
/* }}} */

/* {{{ proto array pg_version([resource connection])
   Returns an array with client, protocol and server version (when available) */
PHP_FUNCTION(pg_version)
{
	php_pgsql_get_link_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_VERSION);
}
/* }}} */

#if HAVE_PQPARAMETERSTATUS
/* {{{ proto string|false pg_parameter_status([resource connection,] string param_name)
   Returns the value of a server parameter */
PHP_FUNCTION(pg_parameter_status)
{
	zval *pgsql_link;
	int id;
	PGconn *pgsql;
	char *param;
	size_t len;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "rs", &pgsql_link, &param, &len) == SUCCESS) {
		id = -1;
	} else if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &param, &len) == SUCCESS) {
		pgsql_link = NULL;
		id = FETCH_DEFAULT_LINK();
	} else {
		RETURN_FALSE;
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	param = (char*)PQparameterStatus(pgsql, param);
	if (param) {
		RETURN_STRING(param);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
#endif

/* {{{ proto bool pg_ping([resource connection])
   Ping database. If connection is bad, try to reconnect. */
PHP_FUNCTION(pg_ping)
{
	zval *pgsql_link;
	int id;
	PGconn *pgsql;
	PGresult *res;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r", &pgsql_link) == SUCCESS) {
		id = -1;
	} else {
		pgsql_link = NULL;
		id = FETCH_DEFAULT_LINK();
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	/* ping connection */
	res = PQexec(pgsql, "SELECT 1;");
	PQclear(res);

	/* check status. */
	if (PQstatus(pgsql) == CONNECTION_OK)
		RETURN_TRUE;

	/* reset connection if it's broken */
	PQreset(pgsql);
	if (PQstatus(pgsql) == CONNECTION_OK) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto resource pg_query([resource connection,] string query)
   Execute a query */
PHP_FUNCTION(pg_query)
{
	zval *pgsql_link = NULL;
	char *query;
	int id = -1, argc = ZEND_NUM_ARGS();
	size_t query_len;
	int leftover = 0;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	pgsql_result_handle *pg_result;

	if (argc == 1) {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &query, &query_len) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &pgsql_link, &query, &query_len) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL, E_NOTICE,"Cannot set connection to blocking mode");
		RETURN_FALSE;
	}
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL, E_NOTICE, "Found results on this connection. Use pg_get_result() to get these results first");
	}
	pgsql_result = PQexec(pgsql, query);
	if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
		PQclear(pgsql_result);
		PQreset(pgsql);
		pgsql_result = PQexec(pgsql, query);
	}

	if (pgsql_result) {
		status = PQresultStatus(pgsql_result);
	} else {
		status = (ExecStatusType) PQstatus(pgsql);
	}

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
/* }}} */

#if HAVE_PQEXECPARAMS || HAVE_PQEXECPREPARED || HAVE_PQSENDQUERYPARAMS || HAVE_PQSENDQUERYPREPARED
/* {{{ _php_pgsql_free_params */
static void _php_pgsql_free_params(char **params, int num_params)
{
	if (num_params > 0) {
		int i;
		for (i = 0; i < num_params; i++) {
			if (params[i]) {
				efree(params[i]);
			}
		}
		efree(params);
	}
}
/* }}} */
#endif

#if HAVE_PQEXECPARAMS
/* {{{ proto resource pg_query_params([resource connection,] string query, array params)
   Execute a query */
PHP_FUNCTION(pg_query_params)
{
	zval *pgsql_link = NULL;
	zval *pv_param_arr, *tmp;
	char *query;
	size_t query_len;
	int id = -1, argc = ZEND_NUM_ARGS();
	int leftover = 0;
	int num_params = 0;
	char **params = NULL;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	pgsql_result_handle *pg_result;
	
	if (argc == 2) {
		if (zend_parse_parameters(argc, "sa", &query, &query_len, &pv_param_arr) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rsa", &pgsql_link, &query, &query_len, &pv_param_arr) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL, E_NOTICE,"Cannot set connection to blocking mode");
		RETURN_FALSE;
	}
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL, E_NOTICE, "Found results on this connection. Use pg_get_result() to get these results first");
	}

	num_params = zend_hash_num_elements(Z_ARRVAL_P(pv_param_arr));
	if (num_params > 0) {
		int i = 0;
		params = (char **)safe_emalloc(sizeof(char *), num_params, 0);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pv_param_arr), tmp) {

			if (Z_TYPE_P(tmp) == IS_NULL) {
				params[i] = NULL;
			} else {
				zval tmp_val;

				ZVAL_COPY(&tmp_val, tmp);
				convert_to_cstring(&tmp_val);
				if (Z_TYPE(tmp_val) != IS_STRING) {
					php_error_docref(NULL, E_WARNING,"Error converting parameter");
					zval_ptr_dtor(&tmp_val);
					_php_pgsql_free_params(params, num_params);
					RETURN_FALSE;
				}
				params[i] = estrndup(Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
				zval_ptr_dtor(&tmp_val);
			}
			i++;
		} ZEND_HASH_FOREACH_END();
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
/* }}} */
#endif

#if HAVE_PQPREPARE
/* {{{ proto resource pg_prepare([resource connection,] string stmtname, string query)
   Prepare a query for future execution */
PHP_FUNCTION(pg_prepare)
{
	zval *pgsql_link = NULL;
	char *query, *stmtname;
	size_t query_len, stmtname_len;
	int id = -1, argc = ZEND_NUM_ARGS();
	int leftover = 0;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	pgsql_result_handle *pg_result;

	if (argc == 2) {
		if (zend_parse_parameters(argc, "ss", &stmtname, &stmtname_len, &query, &query_len) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rss", &pgsql_link, &stmtname, &stmtname_len, &query, &query_len) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL, E_NOTICE,"Cannot set connection to blocking mode");
		RETURN_FALSE;
	}
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL, E_NOTICE, "Found results on this connection. Use pg_get_result() to get these results first");
	}
	pgsql_result = PQprepare(pgsql, stmtname, query, 0, NULL);
	if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
		PQclear(pgsql_result);
		PQreset(pgsql);
		pgsql_result = PQprepare(pgsql, stmtname, query, 0, NULL);
	}

	if (pgsql_result) {
		status = PQresultStatus(pgsql_result);
	} else {
		status = (ExecStatusType) PQstatus(pgsql);
	}

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
/* }}} */
#endif

#if HAVE_PQEXECPREPARED
/* {{{ proto resource pg_execute([resource connection,] string stmtname, array params)
   Execute a prepared query  */
PHP_FUNCTION(pg_execute)
{
	zval *pgsql_link = NULL;
	zval *pv_param_arr, *tmp;
	char *stmtname;
	size_t stmtname_len;
	int id = -1, argc = ZEND_NUM_ARGS();
	int leftover = 0;
	int num_params = 0;
	char **params = NULL;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	pgsql_result_handle *pg_result;

	if (argc == 2) {
		if (zend_parse_parameters(argc, "sa/", &stmtname, &stmtname_len, &pv_param_arr)==FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rsa/", &pgsql_link, &stmtname, &stmtname_len, &pv_param_arr) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL, E_NOTICE,"Cannot set connection to blocking mode");
		RETURN_FALSE;
	}
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
		leftover = 1;
	}
	if (leftover) {
		php_error_docref(NULL, E_NOTICE, "Found results on this connection. Use pg_get_result() to get these results first");
	}

	num_params = zend_hash_num_elements(Z_ARRVAL_P(pv_param_arr));
	if (num_params > 0) {
		int i = 0;
		params = (char **)safe_emalloc(sizeof(char *), num_params, 0);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pv_param_arr), tmp) {

			if (Z_TYPE_P(tmp) == IS_NULL) {
				params[i] = NULL;
			} else {
				zval tmp_val;

				ZVAL_COPY(&tmp_val, tmp);
				convert_to_string(&tmp_val);
				if (Z_TYPE(tmp_val) != IS_STRING) {
					php_error_docref(NULL, E_WARNING,"Error converting parameter");
					zval_ptr_dtor(&tmp_val);
					_php_pgsql_free_params(params, num_params);
					RETURN_FALSE;
				}
				params[i] = estrndup(Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
				zval_ptr_dtor(&tmp_val);
			}

			i++;
		} ZEND_HASH_FOREACH_END();
	}

	pgsql_result = PQexecPrepared(pgsql, stmtname, num_params, 
					(const char * const *)params, NULL, NULL, 0);
	if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
		PQclear(pgsql_result);
		PQreset(pgsql);
		pgsql_result = PQexecPrepared(pgsql, stmtname, num_params, 
						(const char * const *)params, NULL, NULL, 0);
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
/* }}} */
#endif

#define PHP_PG_NUM_ROWS 1
#define PHP_PG_NUM_FIELDS 2
#define PHP_PG_CMD_TUPLES 3

/* {{{ php_pgsql_get_result_info
 */
static void php_pgsql_get_result_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	switch (entry_type) {
		case PHP_PG_NUM_ROWS:
			RETVAL_LONG(PQntuples(pgsql_result));
			break;
		case PHP_PG_NUM_FIELDS:
			RETVAL_LONG(PQnfields(pgsql_result));
			break;
		case PHP_PG_CMD_TUPLES:
#if HAVE_PQCMDTUPLES
			RETVAL_LONG(atoi(PQcmdTuples(pgsql_result)));
#else
			php_error_docref(NULL, E_WARNING, "Not supported under this build");
			RETVAL_LONG(0);
#endif
			break;
		default:
			RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto int pg_num_rows(resource result)
   Return the number of rows in the result */
PHP_FUNCTION(pg_num_rows)
{
	php_pgsql_get_result_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_NUM_ROWS);
}
/* }}} */

/* {{{ proto int pg_num_fields(resource result)
   Return the number of fields in the result */
PHP_FUNCTION(pg_num_fields)
{
	php_pgsql_get_result_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_NUM_FIELDS);
}
/* }}} */

#if HAVE_PQCMDTUPLES
/* {{{ proto int pg_affected_rows(resource result)
   Returns the number of affected tuples */
PHP_FUNCTION(pg_affected_rows)
{
	php_pgsql_get_result_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_CMD_TUPLES);
}
/* }}} */
#endif

/* {{{ proto string pg_last_notice(resource connection)
   Returns the last notice set by the backend */
PHP_FUNCTION(pg_last_notice) 
{
	zval *pgsql_link = NULL;
	PGconn *pg_link;
	int id = -1;
	php_pgsql_notice *notice;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL) {
		RETURN_FALSE;
	}

	/* Just to check if user passed valid resoruce */
	ZEND_FETCH_RESOURCE2(pg_link, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if ((notice = zend_hash_index_find_ptr(&PGG(notices), (zend_ulong)Z_RES_HANDLE_P(pgsql_link))) == NULL) {
		RETURN_FALSE;
	}
	RETURN_STRINGL(notice->message, notice->len);
}
/* }}} */

/* {{{ get_field_name
 */
static char *get_field_name(PGconn *pgsql, Oid oid, HashTable *list)
{
	PGresult *result;
	smart_str str = {0};
	zend_resource *field_type;
	char *ret=NULL;

	/* try to lookup the type in the resource list */
	smart_str_appends(&str, "pgsql_oid_");
	smart_str_append_unsigned(&str, oid);
	smart_str_0(&str);

	if ((field_type = zend_hash_find_ptr(list, str.s)) != NULL) {
		ret = estrdup((char *)field_type->ptr);
	} else { /* hash all oid's */
		int i, num_rows;
		int oid_offset,name_offset;
		char *tmp_oid, *end_ptr, *tmp_name;
		zend_resource new_oid_entry;

		if ((result = PQexec(pgsql, "select oid,typname from pg_type")) == NULL || PQresultStatus(result) != PGRES_TUPLES_OK) {
			if (result) {
				PQclear(result);
			}
			smart_str_free(&str);
			return estrndup("", sizeof("") - 1);
		}
		num_rows = PQntuples(result);
		oid_offset = PQfnumber(result,"oid");
		name_offset = PQfnumber(result,"typname");

		for (i=0; i<num_rows; i++) {
			if ((tmp_oid = PQgetvalue(result,i,oid_offset))==NULL) {
				continue;
			}
			
			str.s->len = 0;
			smart_str_appends(&str, "pgsql_oid_");
			smart_str_appends(&str, tmp_oid);
			smart_str_0(&str);

			if ((tmp_name = PQgetvalue(result,i,name_offset))==NULL) {
				continue;
			}
			new_oid_entry.type = le_string;
			new_oid_entry.ptr = estrdup(tmp_name);
			zend_hash_update_mem(list, str.s, (void *) &new_oid_entry, sizeof(zend_resource));
			if (!ret && strtoul(tmp_oid, &end_ptr, 10)==oid) {
				ret = estrdup(tmp_name);
			}
		}
		PQclear(result);
	}

	smart_str_free(&str);
	return ret;
}
/* }}} */

#ifdef HAVE_PQFTABLE
/* {{{ proto mixed pg_field_table(resource result, int field_number[, bool oid_only])
   Returns the name of the table field belongs to, or table's oid if oid_only is true */
PHP_FUNCTION(pg_field_table)
{
	zval *result;
	pgsql_result_handle *pg_result;
	zend_long fnum = -1;
	zend_bool return_oid = 0;
	Oid oid;
	smart_str hash_key = {0};
	char *table_name;
	zend_resource *field_table;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl|b", &result, &fnum, &return_oid) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	if (fnum < 0 || fnum >= PQnfields(pg_result->result)) {
		php_error_docref(NULL, E_WARNING, "Bad field offset specified");
		RETURN_FALSE;
	}

	oid = PQftable(pg_result->result, (int)fnum);

	if (InvalidOid == oid) {
		RETURN_FALSE;
	}

	if (return_oid) {
#if UINT_MAX > ZEND_LONG_MAX /* Oid is unsigned int, we don't need this code, where LONG is wider */
		if (oid > ZEND_LONG_MAX) {
			smart_str oidstr = {0};
			smart_str_append_unsigned(&oidstr, oid);
			smart_str_0(&oidstr);
			RETURN_STR(oidstr.s);
		} else
#endif
			RETURN_LONG((zend_long)oid);
	}

	/* try to lookup the table name in the resource list */
	smart_str_appends(&hash_key, "pgsql_table_oid_");
	smart_str_append_unsigned(&hash_key, oid);
	smart_str_0(&hash_key);

	if ((field_table = zend_hash_find_ptr(&EG(regular_list), hash_key.s)) != NULL) {
		smart_str_free(&hash_key);
		RETURN_STRING((char *)field_table->ptr);
	} else { /* Not found, lookup by querying PostgreSQL system tables */
		PGresult *tmp_res;
		smart_str querystr = {0};
		zend_resource new_field_table;

		smart_str_appends(&querystr, "select relname from pg_class where oid=");
		smart_str_append_unsigned(&querystr, oid);
		smart_str_0(&querystr);

		if ((tmp_res = PQexec(pg_result->conn, querystr.s->val)) == NULL || PQresultStatus(tmp_res) != PGRES_TUPLES_OK) {
			if (tmp_res) {
				PQclear(tmp_res);
			}
			smart_str_free(&querystr);
			smart_str_free(&hash_key);
			RETURN_FALSE;
		}

		smart_str_free(&querystr);

		if ((table_name = PQgetvalue(tmp_res, 0, 0)) == NULL) {
			PQclear(tmp_res);
			smart_str_free(&hash_key);
			RETURN_FALSE;
		}

		new_field_table.type = le_string;
		new_field_table.ptr = estrdup(table_name);
		zend_hash_update_mem(&EG(regular_list), hash_key.s, (void *)&new_field_table, sizeof(zend_resource));

		smart_str_free(&hash_key);
		PQclear(tmp_res);
		RETURN_STRING(table_name);
	}

}
/* }}} */
#endif

#define PHP_PG_FIELD_NAME 1
#define PHP_PG_FIELD_SIZE 2
#define PHP_PG_FIELD_TYPE 3
#define PHP_PG_FIELD_TYPE_OID 4

/* {{{ php_pgsql_get_field_info
 */
static void php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result;
	zend_long field;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	Oid oid;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &result, &field) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	
	if (field < 0 || field >= PQnfields(pgsql_result)) {
		php_error_docref(NULL, E_WARNING, "Bad field offset specified");
		RETURN_FALSE;
	}

	switch (entry_type) {
		case PHP_PG_FIELD_NAME:
			RETURN_STRING(PQfname(pgsql_result, (int)field));
			break;
		case PHP_PG_FIELD_SIZE:
			RETURN_LONG(PQfsize(pgsql_result, (int)field));
			break;
		case PHP_PG_FIELD_TYPE: {
				char *name = get_field_name(pg_result->conn, PQftype(pgsql_result, (int)field), &EG(regular_list));
				RETVAL_STRING(name);
				efree(name);
			}
			break;
		case PHP_PG_FIELD_TYPE_OID:
			
			oid = PQftype(pgsql_result, (int)field);
#if UINT_MAX > ZEND_LONG_MAX
			if (oid > ZEND_LONG_MAX) {
				smart_str s = {0};
				smart_str_append_unsigned(&s, oid);
				smart_str_0(&s);
				RETURN_STR(s.s);
			} else
#endif
			{
				RETURN_LONG((zend_long)oid);
			}
			break;
		default:
			RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto string pg_field_name(resource result, int field_number)
   Returns the name of the field */
PHP_FUNCTION(pg_field_name)
{
	php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_FIELD_NAME);
}
/* }}} */

/* {{{ proto int pg_field_size(resource result, int field_number)
   Returns the internal size of the field */ 
PHP_FUNCTION(pg_field_size)
{
	php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_FIELD_SIZE);
}
/* }}} */

/* {{{ proto string pg_field_type(resource result, int field_number)
   Returns the type name for the given field */
PHP_FUNCTION(pg_field_type)
{
	php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_FIELD_TYPE);
}
/* }}} */

/* {{{ proto string pg_field_type_oid(resource result, int field_number)
   Returns the type oid for the given field */
PHP_FUNCTION(pg_field_type_oid)
{
	php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_FIELD_TYPE_OID);
}
/* }}} */

/* {{{ proto int pg_field_num(resource result, string field_name)
   Returns the field number of the named field */
PHP_FUNCTION(pg_field_num)
{
	zval *result;
	char *field;
	size_t field_len;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &result, &field, &field_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	RETURN_LONG(PQfnumber(pgsql_result, field));
}
/* }}} */

/* {{{ proto mixed pg_fetch_result(resource result, [int row_number,] mixed field_name)
   Returns values from a result identifier */
PHP_FUNCTION(pg_fetch_result)
{
	zval *result, *field=NULL;
	zend_long row;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	int field_offset, pgsql_row, argc = ZEND_NUM_ARGS();

	if (argc == 2) {
		if (zend_parse_parameters(argc, "rz", &result, &field) == FAILURE) {
			return;
		}
	} else {
		if (zend_parse_parameters(argc, "rlz", &result, &row, &field) == FAILURE) {
			return;
		}
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	if (argc == 2) {
		if (pg_result->row < 0) {
			pg_result->row = 0;
		}
		pgsql_row = pg_result->row;
		if (pgsql_row >= PQntuples(pgsql_result)) {
			RETURN_FALSE;
		}
	} else {
		if (row < 0 || row >= PQntuples(pgsql_result)) {
			php_error_docref(NULL, E_WARNING, "Unable to jump to row %pd on PostgreSQL result index %pd",
							row, Z_LVAL_P(result));
			RETURN_FALSE;
		}
		pgsql_row = (int)row;
	}
	switch (Z_TYPE_P(field)) {
		case IS_STRING:
			field_offset = PQfnumber(pgsql_result, Z_STRVAL_P(field));
			if (field_offset < 0 || field_offset >= PQnfields(pgsql_result)) {
				php_error_docref(NULL, E_WARNING, "Bad column offset specified");
				RETURN_FALSE;
			}
			break;
		default:
			convert_to_long_ex(field);
			if (Z_LVAL_P(field) < 0 || Z_LVAL_P(field) >= PQnfields(pgsql_result)) {
				php_error_docref(NULL, E_WARNING, "Bad column offset specified");
				RETURN_FALSE;
			}
			field_offset = (int)Z_LVAL_P(field);
			break;
	}

	if (PQgetisnull(pgsql_result, pgsql_row, field_offset)) {
		RETVAL_NULL();
	} else {
		RETVAL_STRINGL(PQgetvalue(pgsql_result, pgsql_row, field_offset),
				PQgetlength(pgsql_result, pgsql_row, field_offset));
	}
}
/* }}} */

/* {{{ void php_pgsql_fetch_hash */
static void php_pgsql_fetch_hash(INTERNAL_FUNCTION_PARAMETERS, zend_long result_type, int into_object)
{
	zval                *result, *zrow = NULL;
	PGresult            *pgsql_result;
	pgsql_result_handle *pg_result;
	int             i, num_fields, pgsql_row, use_row;
	zend_long            row = -1;
	char            *field_name;
	zval            *ctor_params = NULL;
	zend_class_entry *ce = NULL;

	if (into_object) {
		zend_string *class_name = NULL;

		if (zend_parse_parameters(ZEND_NUM_ARGS(), "r|z!Sz", &result, &zrow, &class_name, &ctor_params) == FAILURE) {
			return;
		}
		if (!class_name) {
			ce = zend_standard_class_def;
		} else {
			ce = zend_fetch_class(class_name, ZEND_FETCH_CLASS_AUTO);
		}
		if (!ce) {
			php_error_docref(NULL, E_WARNING, "Could not find class '%s'", class_name->val);
			return;
		}
		result_type = PGSQL_ASSOC;
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "r|z!l", &result, &zrow, &result_type) == FAILURE) {
			return;
		}
	}
	if (zrow == NULL) {
		row = -1;
	} else {
		convert_to_long(zrow);
		row = Z_LVAL_P(zrow);
		if (row < 0) {
			php_error_docref(NULL, E_WARNING, "The row parameter must be greater or equal to zero");
			RETURN_FALSE;
		}
	}
	use_row = ZEND_NUM_ARGS() > 1 && row != -1;

	if (!(result_type & PGSQL_BOTH)) {
		php_error_docref(NULL, E_WARNING, "Invalid result type");
		RETURN_FALSE;
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	if (use_row) { 
		if (row < 0 || row >= PQntuples(pgsql_result)) {
			php_error_docref(NULL, E_WARNING, "Unable to jump to row %pd on PostgreSQL result index %pd",
							row, Z_LVAL_P(result));
			RETURN_FALSE;
		}
		pgsql_row = (int)row;
		pg_result->row = pgsql_row;
	} else {
		/* If 2nd param is NULL, use internal row counter to access next row */
		pgsql_row = pg_result->row;
		if (pgsql_row < 0 || pgsql_row >= PQntuples(pgsql_result)) {
			RETURN_FALSE;
		}
		pg_result->row++;
	}

	array_init(return_value);
	for (i = 0, num_fields = PQnfields(pgsql_result); i < num_fields; i++) {
		if (PQgetisnull(pgsql_result, pgsql_row, i)) {
			if (result_type & PGSQL_NUM) {
				add_index_null(return_value, i);
			}
			if (result_type & PGSQL_ASSOC) {
				field_name = PQfname(pgsql_result, i);
				add_assoc_null(return_value, field_name);
			}
		} else {
			char *element = PQgetvalue(pgsql_result, pgsql_row, i);
			if (element) {
				const size_t element_len = strlen(element);

				if (result_type & PGSQL_NUM) {
					add_index_stringl(return_value, i, element, element_len);
				}

				if (result_type & PGSQL_ASSOC) {
					field_name = PQfname(pgsql_result, i);
					add_assoc_stringl(return_value, field_name, element, element_len);
				}
			}
		}
	}

	if (into_object) {
		zval dataset;
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
		zval retval;

		ZVAL_COPY_VALUE(&dataset, return_value);
		object_and_properties_init(return_value, ce, NULL);
		if (!ce->default_properties_count && !ce->__set) {
			ALLOC_HASHTABLE(Z_OBJ_P(return_value)->properties);
			*Z_OBJ_P(return_value)->properties = *Z_ARRVAL(dataset);
			efree(Z_ARR(dataset));
		} else {
			zend_merge_properties(return_value, Z_ARRVAL(dataset));
			zval_ptr_dtor(&dataset);
		}

		if (ce->constructor) {
			fci.size = sizeof(fci);
			fci.function_table = &ce->function_table;
			ZVAL_UNDEF(&fci.function_name);
			fci.symbol_table = NULL;
			fci.object = Z_OBJ_P(return_value);
			fci.retval = &retval;
			fci.params = NULL;
			fci.param_count = 0;
			fci.no_separation = 1;

			if (ctor_params && Z_TYPE_P(ctor_params) != IS_NULL) {
				if (zend_fcall_info_args(&fci, ctor_params) == FAILURE) {
					/* Two problems why we throw exceptions here: PHP is typeless
					 * and hence passing one argument that's not an array could be
					 * by mistake and the other way round is possible, too. The
					 * single value is an array. Also we'd have to make that one
					 * argument passed by reference.
					 */
					zend_throw_exception(zend_exception_get_default(), "Parameter ctor_params must be an array", 0);
					return;
				}
			}

			fcc.initialized = 1;
			fcc.function_handler = ce->constructor;
			fcc.calling_scope = EG(scope);
			fcc.called_scope = Z_OBJCE_P(return_value);
			fcc.object = Z_OBJ_P(return_value);

			if (zend_call_function(&fci, &fcc) == FAILURE) {
				zend_throw_exception_ex(zend_exception_get_default(), 0, "Could not execute %s::%s()", ce->name, ce->constructor->common.function_name);
			} else {
				zval_ptr_dtor(&retval);
			}
			if (fci.params) {
				efree(fci.params);
			}
		} else if (ctor_params) {
			zend_throw_exception_ex(zend_exception_get_default(), 0, "Class %s does not have a constructor hence you cannot use ctor_params", ce->name);
		}
	}
}
/* }}} */

/* {{{ proto array pg_fetch_row(resource result [, int row [, int result_type]])
   Get a row as an enumerated array */ 
PHP_FUNCTION(pg_fetch_row)
{
	php_pgsql_fetch_hash(INTERNAL_FUNCTION_PARAM_PASSTHRU, PGSQL_NUM, 0);
}
/* }}} */

/* {{{ proto array pg_fetch_assoc(resource result [, int row])
   Fetch a row as an assoc array */
PHP_FUNCTION(pg_fetch_assoc)
{
	/* pg_fetch_assoc() is added from PHP 4.3.0. It should raise error, when
	   there is 3rd parameter */
	if (ZEND_NUM_ARGS() > 2)
		WRONG_PARAM_COUNT;
	php_pgsql_fetch_hash(INTERNAL_FUNCTION_PARAM_PASSTHRU, PGSQL_ASSOC, 0);
}
/* }}} */

/* {{{ proto array pg_fetch_array(resource result [, int row [, int result_type]])
   Fetch a row as an array */
PHP_FUNCTION(pg_fetch_array)
{
	php_pgsql_fetch_hash(INTERNAL_FUNCTION_PARAM_PASSTHRU, PGSQL_BOTH, 0);
}
/* }}} */

/* {{{ proto object pg_fetch_object(resource result [, int row [, string class_name [, NULL|array ctor_params]]])
   Fetch a row as an object */
PHP_FUNCTION(pg_fetch_object)
{
	/* pg_fetch_object() allowed result_type used to be. 3rd parameter
	   must be allowed for compatibility */
	php_pgsql_fetch_hash(INTERNAL_FUNCTION_PARAM_PASSTHRU, PGSQL_ASSOC, 1);
}
/* }}} */

/* {{{ proto array pg_fetch_all(resource result)
   Fetch all rows into array */
PHP_FUNCTION(pg_fetch_all)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	array_init(return_value);
	if (php_pgsql_result2array(pgsql_result, return_value) == FAILURE) {
		zval_dtor(return_value);
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto array pg_fetch_all_columns(resource result [, int column_number])
   Fetch all rows into array */
PHP_FUNCTION(pg_fetch_all_columns)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	zend_long colno=0;
	int pg_numrows, pg_row;
	size_t num_fields;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r|l", &result, &colno) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;

	num_fields = PQnfields(pgsql_result);
	if (colno >= (zend_long)num_fields || colno < 0) {
		php_error_docref(NULL, E_WARNING, "Invalid column number '%pd'", colno);
		RETURN_FALSE;
	}

	array_init(return_value);

	if ((pg_numrows = PQntuples(pgsql_result)) <= 0) {
		return;
	}

	for (pg_row = 0; pg_row < pg_numrows; pg_row++) {
		if (PQgetisnull(pgsql_result, pg_row, (int)colno)) {
			add_next_index_null(return_value);
		} else {
			add_next_index_string(return_value, PQgetvalue(pgsql_result, pg_row, (int)colno)); 
		}
	}
}
/* }}} */

/* {{{ proto bool pg_result_seek(resource result, int offset)
   Set internal row offset */
PHP_FUNCTION(pg_result_seek)
{
	zval *result;
	zend_long row;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rl", &result, &row) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	if (row < 0 || row >= PQntuples(pg_result->result)) {
		RETURN_FALSE;
	}

	/* seek to offset */
	pg_result->row = (int)row;
	RETURN_TRUE;
}
/* }}} */

#define PHP_PG_DATA_LENGTH 1
#define PHP_PG_DATA_ISNULL 2

/* {{{ php_pgsql_data_info
 */
static void php_pgsql_data_info(INTERNAL_FUNCTION_PARAMETERS, int entry_type)
{
	zval *result, *field;
	zend_long row;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	int field_offset, pgsql_row, argc = ZEND_NUM_ARGS();

	if (argc == 2) {
		if (zend_parse_parameters(argc, "rz", &result, &field) == FAILURE) {
			return;
		}
	} else {
		if (zend_parse_parameters(argc, "rlz", &result, &row, &field) == FAILURE) {
			return;
		}
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	if (argc == 2) {
		if (pg_result->row < 0) {
			pg_result->row = 0;
		}
		pgsql_row = pg_result->row;
		if (pgsql_row < 0 || pgsql_row >= PQntuples(pgsql_result)) {
			RETURN_FALSE;
		}
	} else {
		if (row < 0 || row >= PQntuples(pgsql_result)) {
			php_error_docref(NULL, E_WARNING, "Unable to jump to row %pd on PostgreSQL result index %pd",
							row, Z_LVAL_P(result));
			RETURN_FALSE;
		}
		pgsql_row = (int)row;
	}

	switch (Z_TYPE_P(field)) {
		case IS_STRING:
			convert_to_string_ex(field);
			field_offset = PQfnumber(pgsql_result, Z_STRVAL_P(field));
			if (field_offset < 0 || field_offset >= PQnfields(pgsql_result)) {
				php_error_docref(NULL, E_WARNING, "Bad column offset specified");
				RETURN_FALSE;
			}
			break;
		default:
			convert_to_long_ex(field);
			if (Z_LVAL_P(field) < 0 || Z_LVAL_P(field) >= PQnfields(pgsql_result)) {
				php_error_docref(NULL, E_WARNING, "Bad column offset specified");
				RETURN_FALSE;
			}
			field_offset = (int)Z_LVAL_P(field);
			break;
	}

	switch (entry_type) {
		case PHP_PG_DATA_LENGTH:
			RETVAL_LONG(PQgetlength(pgsql_result, pgsql_row, field_offset));
			break;
		case PHP_PG_DATA_ISNULL:
			RETVAL_LONG(PQgetisnull(pgsql_result, pgsql_row, field_offset))
			break;
	}
}
/* }}} */

/* {{{ proto int pg_field_prtlen(resource result, [int row,] mixed field_name_or_number)
   Returns the printed length */
PHP_FUNCTION(pg_field_prtlen)
{
	php_pgsql_data_info(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_PG_DATA_LENGTH);
}
/* }}} */

/* {{{ proto int pg_field_is_null(resource result, [int row,] mixed field_name_or_number)
   Test if a field is NULL */
PHP_FUNCTION(pg_field_is_null)
{
	php_pgsql_data_info(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_PG_DATA_ISNULL);
}
/* }}} */

/* {{{ proto bool pg_free_result(resource result)
   Free result memory */
PHP_FUNCTION(pg_free_result)
{
	zval *result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);
	if (Z_RES_P(result) == NULL) {
		RETURN_FALSE;
	}
	zend_list_close(Z_RES_P(result));
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto string pg_last_oid(resource result)
   Returns the last object identifier */
PHP_FUNCTION(pg_last_oid)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
#ifdef HAVE_PQOIDVALUE
	Oid oid;
#endif

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &result) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);
	pgsql_result = pg_result->result;
#ifdef HAVE_PQOIDVALUE
	oid = PQoidValue(pgsql_result);
	if (oid == InvalidOid) {
		RETURN_FALSE;
	}
	PGSQL_RETURN_OID(oid);
#else
	Z_STRVAL_P(return_value) = (char *) PQoidStatus(pgsql_result);
	if (Z_STRVAL_P(return_value)) {
		RETURN_STRING(Z_STRVAL_P(return_value));
	}
	RETURN_EMPTY_STRING();
#endif
}
/* }}} */

/* {{{ proto bool pg_trace(string filename [, string mode [, resource connection]])
   Enable tracing a PostgreSQL connection */
PHP_FUNCTION(pg_trace)
{
	char *z_filename, *mode = "w";
	size_t z_filename_len, mode_len;
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;
	FILE *fp = NULL;
	php_stream *stream;
	id = FETCH_DEFAULT_LINK();

	if (zend_parse_parameters(argc, "s|sr", &z_filename, &z_filename_len, &mode, &mode_len, &pgsql_link) == FAILURE) {
		return;
	}

	if (argc < 3) {
		CHECK_DEFAULT_LINK(id);
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	stream = php_stream_open_wrapper(z_filename, mode, REPORT_ERRORS, NULL);

	if (!stream) {
		RETURN_FALSE;
	}

	if (FAILURE == php_stream_cast(stream, PHP_STREAM_AS_STDIO, (void**)&fp, REPORT_ERRORS))	{
		php_stream_close(stream);
		RETURN_FALSE;
	}
	php_stream_auto_cleanup(stream);
	PQtrace(pgsql, fp);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool pg_untrace([resource connection])
   Disable tracing of a PostgreSQL connection */
PHP_FUNCTION(pg_untrace)
{
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;
	
	if (zend_parse_parameters(argc, "|r", &pgsql_link) == FAILURE) {
		return;
	}

	if (argc == 0) { 
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	PQuntrace(pgsql);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto mixed pg_lo_create([resource connection],[mixed large_object_oid])
   Create a large object */
PHP_FUNCTION(pg_lo_create)
{
	zval *pgsql_link = NULL, *oid = NULL;
	PGconn *pgsql;
	Oid pgsql_oid, wanted_oid = InvalidOid;
	int id = -1, argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "|zz", &pgsql_link, &oid) == FAILURE) {
		return;
	}

	if ((argc == 1) && (Z_TYPE_P(pgsql_link) != IS_RESOURCE)) {
		oid = pgsql_link;
		pgsql_link = NULL;
	}
	
	if (pgsql_link == NULL) {
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
		if (id == -1) {
			RETURN_FALSE;
		}
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	if (oid) {
#ifndef HAVE_PG_LO_CREATE
		php_error_docref(NULL, E_NOTICE, "Passing OID value is not supported. Upgrade your PostgreSQL");
#else
		switch (Z_TYPE_P(oid)) {
		case IS_STRING:
			{
				char *end_ptr;
				wanted_oid = (Oid)strtoul(Z_STRVAL_P(oid), &end_ptr, 10);
				if ((Z_STRVAL_P(oid)+Z_STRLEN_P(oid)) != end_ptr) {
				/* wrong integer format */
				php_error_docref(NULL, E_NOTICE, "invalid OID value passed");
				RETURN_FALSE;
				}
			}
			break;
		case IS_LONG:
			if (Z_LVAL_P(oid) < (zend_long)InvalidOid) {
				php_error_docref(NULL, E_NOTICE, "invalid OID value passed");
				RETURN_FALSE;
			}
			wanted_oid = (Oid)Z_LVAL_P(oid);
			break;
		default:
			php_error_docref(NULL, E_NOTICE, "invalid OID value passed");
			RETURN_FALSE;
        }
		if ((pgsql_oid = lo_create(pgsql, wanted_oid)) == InvalidOid) {
			php_error_docref(NULL, E_WARNING, "Unable to create PostgreSQL large object");
			RETURN_FALSE;
		}

		PGSQL_RETURN_OID(pgsql_oid);
#endif
	}

	if ((pgsql_oid = lo_creat(pgsql, INV_READ|INV_WRITE)) == InvalidOid) {
		php_error_docref(NULL, E_WARNING, "Unable to create PostgreSQL large object");
		RETURN_FALSE;
	}

	PGSQL_RETURN_OID(pgsql_oid);
}
/* }}} */

/* {{{ proto bool pg_lo_unlink([resource connection,] string large_object_oid)
   Delete a large object */
PHP_FUNCTION(pg_lo_unlink)
{
	zval *pgsql_link = NULL;
	zend_long oid_long;
	char *oid_string, *end_ptr;
	size_t oid_strlen;
	PGconn *pgsql;
	Oid oid;
	int id = -1;
	int argc = ZEND_NUM_ARGS();

	/* accept string type since Oid type is unsigned int */
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rs", &pgsql_link, &oid_string, &oid_strlen) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rl", &pgsql_link, &oid_long) == SUCCESS) {
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "s", &oid_string, &oid_strlen) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "l", &oid_long) == SUCCESS) {
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID is specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	else {
		php_error_docref(NULL, E_WARNING, "Requires 1 or 2 arguments");
		RETURN_FALSE;
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (lo_unlink(pgsql, oid) == -1) {
		php_error_docref(NULL, E_WARNING, "Unable to delete PostgreSQL large object %u", oid);
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto resource pg_lo_open([resource connection,] int large_object_oid, string mode)
   Open a large object and return fd */
PHP_FUNCTION(pg_lo_open)
{
	zval *pgsql_link = NULL;
	zend_long oid_long;
	char *oid_string, *end_ptr, *mode_string;
	size_t oid_strlen, mode_strlen;
	PGconn *pgsql;
	Oid oid;
	int id = -1, pgsql_mode=0, pgsql_lofd;
	int create = 0;
	pgLofp *pgsql_lofp;
	int argc = ZEND_NUM_ARGS();

	/* accept string type since Oid is unsigned int */
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rss", &pgsql_link, &oid_string, &oid_strlen, &mode_string, &mode_strlen) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rls", &pgsql_link, &oid_long, &mode_string, &mode_strlen) == SUCCESS) {
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "ss", &oid_string, &oid_strlen, &mode_string, &mode_strlen) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "ls", &oid_long, &mode_string, &mode_strlen) == SUCCESS) {
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	else {
		php_error_docref(NULL, E_WARNING, "Requires 1 or 2 arguments");
		RETURN_FALSE;
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	/* r/w/+ is little bit more PHP-like than INV_READ/INV_WRITE and a lot of
	   faster to type. Unfortunately, doesn't behave the same way as fopen()...
	   (Jouni)
	*/

	if (strchr(mode_string, 'r') == mode_string) {
		pgsql_mode |= INV_READ;
		if (strchr(mode_string, '+') == mode_string+1) {
			pgsql_mode |= INV_WRITE;
		}
	}
	if (strchr(mode_string, 'w') == mode_string) {
		pgsql_mode |= INV_WRITE;
		create = 1;
		if (strchr(mode_string, '+') == mode_string+1) {
			pgsql_mode |= INV_READ;
		}
	}

	pgsql_lofp = (pgLofp *) emalloc(sizeof(pgLofp));

	if ((pgsql_lofd = lo_open(pgsql, oid, pgsql_mode)) == -1) {
		if (create) {
			if ((oid = lo_creat(pgsql, INV_READ|INV_WRITE)) == 0) {
				efree(pgsql_lofp);
				php_error_docref(NULL, E_WARNING, "Unable to create PostgreSQL large object");
				RETURN_FALSE;
			} else {
				if ((pgsql_lofd = lo_open(pgsql, oid, pgsql_mode)) == -1) {
					if (lo_unlink(pgsql, oid) == -1) {
						efree(pgsql_lofp);
						php_error_docref(NULL, E_WARNING, "Something is really messed up! Your database is badly corrupted in a way NOT related to PHP");
						RETURN_FALSE;
					}
					efree(pgsql_lofp);
					php_error_docref(NULL, E_WARNING, "Unable to open PostgreSQL large object");
					RETURN_FALSE;
				} else {
					pgsql_lofp->conn = pgsql;
					pgsql_lofp->lofd = pgsql_lofd;
					ZEND_REGISTER_RESOURCE(return_value, pgsql_lofp, le_lofp);
				}
			}
		} else {
			efree(pgsql_lofp);
			php_error_docref(NULL, E_WARNING, "Unable to open PostgreSQL large object");
			RETURN_FALSE;
		}
	} else {
		pgsql_lofp->conn = pgsql;
		pgsql_lofp->lofd = pgsql_lofd;
		ZEND_REGISTER_RESOURCE(return_value, pgsql_lofp, le_lofp);
	}
}
/* }}} */

/* {{{ proto bool pg_lo_close(resource large_object)
   Close a large object */
PHP_FUNCTION(pg_lo_close)
{
	zval *pgsql_lofp;
	pgLofp *pgsql;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_lofp) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_lofp, -1, "PostgreSQL large object", le_lofp);
	
	if (lo_close((PGconn *)pgsql->conn, pgsql->lofd) < 0) {
		php_error_docref(NULL, E_WARNING, "Unable to close PostgreSQL large object descriptor %d", pgsql->lofd);
		RETVAL_FALSE;
	} else {
		RETVAL_TRUE;
	}

	zend_list_close(Z_RES_P(pgsql_lofp));
	return;
}
/* }}} */

#define PGSQL_LO_READ_BUF_SIZE  8192

/* {{{ proto string pg_lo_read(resource large_object [, int len])
   Read a large object */
PHP_FUNCTION(pg_lo_read)
{
	zval *pgsql_id;
	zend_long len;
	size_t buf_len = PGSQL_LO_READ_BUF_SIZE;
	int nbytes, argc = ZEND_NUM_ARGS();
	zend_string *buf;
	pgLofp *pgsql;

	if (zend_parse_parameters(argc, "r|l", &pgsql_id, &len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if (argc > 1) {
		buf_len = len < 0 ? 0 : len;
	}
	
	buf = zend_string_alloc(buf_len, 0);
	if ((nbytes = lo_read((PGconn *)pgsql->conn, pgsql->lofd, buf->val, buf->len))<0) {
		zend_string_free(buf);
		RETURN_FALSE;
	}

	buf->len = nbytes;
	buf->val[buf->len] = '\0';
	RETURN_STR(buf);
}
/* }}} */

/* {{{ proto int pg_lo_write(resource large_object, string buf [, int len])
   Write a large object */
PHP_FUNCTION(pg_lo_write)
{
  	zval *pgsql_id;
  	char *str;
  	zend_long z_len;
	size_t str_len, nbytes;
	size_t len;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rs|l", &pgsql_id, &str, &str_len, &z_len) == FAILURE) {
		return;
	}

	if (argc > 2) {
		if (z_len > (zend_long)str_len) {
			php_error_docref(NULL, E_WARNING, "Cannot write more than buffer size %d. Tried to write %pd", str_len, z_len);
			RETURN_FALSE;
		}
		if (z_len < 0) {
			php_error_docref(NULL, E_WARNING, "Buffer size must be larger than 0, but %pd was specified", z_len);
			RETURN_FALSE;
		}
		len = z_len;
	}
	else {
		len = str_len;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

	if ((nbytes = lo_write((PGconn *)pgsql->conn, pgsql->lofd, str, len)) == -1) {
		RETURN_FALSE;
	}

	RETURN_LONG(nbytes);
}
/* }}} */

/* {{{ proto int pg_lo_read_all(resource large_object)
   Read a large object and send straight to browser */
PHP_FUNCTION(pg_lo_read_all)
{
  	zval *pgsql_id;
	int tbytes;
	volatile int nbytes;
	char buf[PGSQL_LO_READ_BUF_SIZE];
	pgLofp *pgsql;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &pgsql_id) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

	tbytes = 0;
	while ((nbytes = lo_read((PGconn *)pgsql->conn, pgsql->lofd, buf, PGSQL_LO_READ_BUF_SIZE))>0) {
		PHPWRITE(buf, nbytes);
		tbytes += nbytes;
	}
	RETURN_LONG(tbytes);
}
/* }}} */

/* {{{ proto int pg_lo_import([resource connection, ] string filename [, mixed oid])
   Import large object direct from filesystem */
PHP_FUNCTION(pg_lo_import)
{
	zval *pgsql_link = NULL, *oid = NULL;
	char *file_in;
	int id = -1;
	size_t name_len;
	int argc = ZEND_NUM_ARGS();
	PGconn *pgsql;
	Oid returned_oid;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rp|z", &pgsql_link, &file_in, &name_len, &oid) == SUCCESS) {
		;
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
									  "p|z", &file_in, &name_len, &oid) == SUCCESS) {
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	/* old calling convention, deprecated since PHP 4.2 */
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
									  "pr", &file_in, &name_len, &pgsql_link ) == SUCCESS) {
		php_error_docref(NULL, E_NOTICE, "Old API is used");
	}
	else {
		WRONG_PARAM_COUNT;
	}
	
	if (php_check_open_basedir(file_in)) {
		RETURN_FALSE;
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (oid) {
#ifndef HAVE_PG_LO_IMPORT_WITH_OID
		php_error_docref(NULL, E_NOTICE, "OID value passing not supported");
#else
		Oid wanted_oid;
		switch (Z_TYPE_P(oid)) {
		case IS_STRING:
			{
				char *end_ptr;
				wanted_oid = (Oid)strtoul(Z_STRVAL_P(oid), &end_ptr, 10);
				if ((Z_STRVAL_P(oid)+Z_STRLEN_P(oid)) != end_ptr) {
				/* wrong integer format */
				php_error_docref(NULL, E_NOTICE, "invalid OID value passed");
				RETURN_FALSE;
				}
			}
			break;
		case IS_LONG:
			if (Z_LVAL_P(oid) < (zend_long)InvalidOid) {
				php_error_docref(NULL, E_NOTICE, "invalid OID value passed");
				RETURN_FALSE;
			}
			wanted_oid = (Oid)Z_LVAL_P(oid);
			break;
		default:
			php_error_docref(NULL, E_NOTICE, "invalid OID value passed");
			RETURN_FALSE;
        }

       returned_oid = lo_import_with_oid(pgsql, file_in, wanted_oid);

	   if (returned_oid == InvalidOid) {
		   RETURN_FALSE;
	   }

	   PGSQL_RETURN_OID(returned_oid);
#endif
	}

	returned_oid = lo_import(pgsql, file_in);

	if (returned_oid == InvalidOid) {
		RETURN_FALSE;
	}
	PGSQL_RETURN_OID(returned_oid);
}
/* }}} */

/* {{{ proto bool pg_lo_export([resource connection, ] int objoid, string filename)
   Export large object direct to filesystem */
PHP_FUNCTION(pg_lo_export)
{
	zval *pgsql_link = NULL;
	char *file_out, *oid_string, *end_ptr;
	size_t oid_strlen;
	int id = -1;
	size_t name_len;
	zend_long oid_long;
	Oid oid;
	PGconn *pgsql;
	int argc = ZEND_NUM_ARGS();

	/* allow string to handle large OID value correctly */
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rlp", &pgsql_link, &oid_long, &file_out, &name_len) == SUCCESS) {
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "rss", &pgsql_link, &oid_string, &oid_strlen, &file_out, &name_len) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
									  "lp",  &oid_long, &file_out, &name_len) == SUCCESS) {
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "sp", &oid_string, &oid_strlen, &file_out, &name_len) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
								 "spr", &oid_string, &oid_strlen, &file_out, &name_len, &pgsql_link) == SUCCESS) {
		oid = (Oid)strtoul(oid_string, &end_ptr, 10);
		if ((oid_string+oid_strlen) != end_ptr) {
			/* wrong integer format */
			php_error_docref(NULL, E_NOTICE, "Wrong OID value passed");
			RETURN_FALSE;
		}
	}
	else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, argc,
									  "lpr", &oid_long, &file_out, &name_len, &pgsql_link) == SUCCESS) {
		php_error_docref(NULL, E_NOTICE, "Old API is used");
		if (oid_long <= InvalidOid) {
			php_error_docref(NULL, E_NOTICE, "Invalid OID specified");
			RETURN_FALSE;
		}
		oid = (Oid)oid_long;
	}
	else {
		php_error_docref(NULL, E_WARNING, "Requires 2 or 3 arguments");
		RETURN_FALSE;
	}
	
	if (php_check_open_basedir(file_out)) {
		RETURN_FALSE;
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (lo_export(pgsql, oid, file_out) == -1) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool pg_lo_seek(resource large_object, int offset [, int whence])
   Seeks position of large object */
PHP_FUNCTION(pg_lo_seek)
{
	zval *pgsql_id = NULL;
	zend_long result, offset = 0, whence = SEEK_CUR;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rl|l", &pgsql_id, &offset, &whence) == FAILURE) {
		return;
	}
	if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
		php_error_docref(NULL, E_WARNING, "Invalid whence parameter");
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

#if HAVE_PG_LO64
	if (PQserverVersion((PGconn *)pgsql->conn) >= 90300) {
		result = lo_lseek64((PGconn *)pgsql->conn, pgsql->lofd, offset, (int)whence);
	} else {
		result = lo_lseek((PGconn *)pgsql->conn, pgsql->lofd, (int)offset, (int)whence);
	}
#else
	result = lo_lseek((PGconn *)pgsql->conn, pgsql->lofd, offset, whence);
#endif
	if (result > -1) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto int pg_lo_tell(resource large_object)
   Returns current position of large object */
PHP_FUNCTION(pg_lo_tell)
{
	zval *pgsql_id = NULL;
	zend_long offset = 0;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "r", &pgsql_id) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

#if HAVE_PG_LO64
	if (PQserverVersion((PGconn *)pgsql->conn) >= 90300) {
		offset = lo_tell64((PGconn *)pgsql->conn, pgsql->lofd);
	} else {
		offset = lo_tell((PGconn *)pgsql->conn, pgsql->lofd);
	}
#else
	offset = lo_tell((PGconn *)pgsql->conn, pgsql->lofd);
#endif
	RETURN_LONG(offset);
}
/* }}} */

#if HAVE_PG_LO_TRUNCATE
/* {{{ proto bool pg_lo_truncate(resource large_object, int size)
   Truncate large object to size */
PHP_FUNCTION(pg_lo_truncate)
{
	zval *pgsql_id = NULL;
	size_t size;
	pgLofp *pgsql;
	int argc = ZEND_NUM_ARGS();
	int result;

	if (zend_parse_parameters(argc, "rl", &pgsql_id, &size) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(pgsql, pgLofp *, pgsql_id, -1, "PostgreSQL large object", le_lofp);

#if HAVE_PG_LO64
	if (PQserverVersion((PGconn *)pgsql->conn) >= 90300) {
		result = lo_truncate64((PGconn *)pgsql->conn, pgsql->lofd, size);
	} else {
		result = lo_truncate((PGconn *)pgsql->conn, pgsql->lofd, size);
	}
#else
	result = lo_truncate((PGconn *)pgsql->conn, pgsql->lofd, size);
#endif
	if (!result) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
#endif

#if HAVE_PQSETERRORVERBOSITY
/* {{{ proto int pg_set_error_verbosity([resource connection,] int verbosity)
   Set error verbosity */
PHP_FUNCTION(pg_set_error_verbosity)
{
	zval *pgsql_link = NULL;
	zend_long verbosity;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;

	if (argc == 1) {
		if (zend_parse_parameters(argc, "l", &verbosity) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rl", &pgsql_link, &verbosity) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}	

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (verbosity & (PQERRORS_TERSE|PQERRORS_DEFAULT|PQERRORS_VERBOSE)) {
		RETURN_LONG(PQsetErrorVerbosity(pgsql, verbosity));
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
#endif

#ifdef HAVE_PQCLIENTENCODING
/* {{{ proto int pg_set_client_encoding([resource connection,] string encoding)
   Set client encoding */
PHP_FUNCTION(pg_set_client_encoding)
{
	char *encoding;
	size_t encoding_len;
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;

	if (argc == 1) {
		if (zend_parse_parameters(argc, "s", &encoding, &encoding_len) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rs", &pgsql_link, &encoding, &encoding_len) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	RETURN_LONG(PQsetClientEncoding(pgsql, encoding));
}
/* }}} */

/* {{{ proto string pg_client_encoding([resource connection])
   Get the current client encoding */
PHP_FUNCTION(pg_client_encoding)
{
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;

	if (zend_parse_parameters(argc, "|r", &pgsql_link) == FAILURE) {
		return;
	}
	
	if (argc == 0) {
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}	

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	/* Just do the same as found in PostgreSQL sources... */

	RETURN_STRING((char *) pg_encoding_to_char(PQclientEncoding(pgsql)));
}
/* }}} */
#endif

#if !HAVE_PQGETCOPYDATA
#define	COPYBUFSIZ	8192
#endif

/* {{{ proto bool pg_end_copy([resource connection])
   Sync with backend. Completes the Copy command */
PHP_FUNCTION(pg_end_copy)
{
	zval *pgsql_link = NULL;
	int id = -1, argc = ZEND_NUM_ARGS();
	PGconn *pgsql;
	int result = 0;

	if (zend_parse_parameters(argc, "|r", &pgsql_link) == FAILURE) {
		return;
	}
	
	if (argc == 0) {
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	result = PQendcopy(pgsql);

	if (result!=0) {
		PHP_PQ_ERROR("Query failed: %s", pgsql);
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto bool pg_put_line([resource connection,] string query)
   Send null-terminated string to backend server*/
PHP_FUNCTION(pg_put_line)
{
	char *query;
	zval *pgsql_link = NULL;
	size_t query_len;
	int id = -1;
	PGconn *pgsql;
	int result = 0, argc = ZEND_NUM_ARGS();

	if (argc == 1) {
		if (zend_parse_parameters(argc, "s", &query, &query_len) == FAILURE) {
			return;
		}
		id = FETCH_DEFAULT_LINK();
		CHECK_DEFAULT_LINK(id);
	} else {
		if (zend_parse_parameters(argc, "rs", &pgsql_link, &query, &query_len) == FAILURE) {
			return;
		}
	}

	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}	

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	result = PQputline(pgsql, query);
	if (result==EOF) {
		PHP_PQ_ERROR("Query failed: %s", pgsql);
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto array pg_copy_to(resource connection, string table_name [, string delimiter [, string null_as]])
   Copy table to array */
PHP_FUNCTION(pg_copy_to)
{
	zval *pgsql_link;
	char *table_name, *pg_delim = NULL, *pg_null_as = NULL;
	size_t table_name_len, pg_delim_len, pg_null_as_len, free_pg_null = 0;
	char *query;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	int copydone = 0;
#if !HAVE_PQGETCOPYDATA
	char copybuf[COPYBUFSIZ];
#endif
	char *csv = (char *)NULL;
	int ret;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rs|ss",
							  &pgsql_link, &table_name, &table_name_len,
							  &pg_delim, &pg_delim_len, &pg_null_as, &pg_null_as_len) == FAILURE) {
		return;
	}
	if (!pg_delim) {
		pg_delim = "\t";
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (!pg_null_as) {
		pg_null_as = estrdup("\\\\N");
		free_pg_null = 1;
	}

	spprintf(&query, 0, "COPY %s TO STDOUT DELIMITERS E'%c' WITH NULL AS E'%s'", table_name, *pg_delim, pg_null_as);

	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
	}
	pgsql_result = PQexec(pgsql, query);
	if (free_pg_null) {
		efree(pg_null_as);
	}
	efree(query);

	if (pgsql_result) {
		status = PQresultStatus(pgsql_result);
	} else {
		status = (ExecStatusType) PQstatus(pgsql);
	}

	switch (status) {
		case PGRES_COPY_OUT:
			if (pgsql_result) {
				PQclear(pgsql_result);
				array_init(return_value);
#if HAVE_PQGETCOPYDATA
				while (!copydone)
				{
					ret = PQgetCopyData(pgsql, &csv, 0);
					switch (ret) {
						case -1:
							copydone = 1;
							break;
						case 0:
						case -2:
							PHP_PQ_ERROR("getline failed: %s", pgsql);
							RETURN_FALSE;
							break;
						default:
							add_next_index_string(return_value, csv);
							PQfreemem(csv);
							break;
					}
				}
#else
				while (!copydone)
				{
					if ((ret = PQgetline(pgsql, copybuf, COPYBUFSIZ))) {
						PHP_PQ_ERROR("getline failed: %s", pgsql);
						RETURN_FALSE;
					}

					if (copybuf[0] == '\\' &&
						copybuf[1] == '.' &&
						copybuf[2] == '\0')
					{
						copydone = 1;
					}
					else
					{
						if (csv == (char *)NULL) {
							csv = estrdup(copybuf);
						} else {
							csv = (char *)erealloc(csv, strlen(csv) + sizeof(char)*(COPYBUFSIZ+1));
							strcat(csv, copybuf);
						}
							
						switch (ret)
						{
							case EOF:
								copydone = 1;
							case 0:
								add_next_index_string(return_value, csv);
								efree(csv);
								csv = (char *)NULL;
								break;
							case 1:
								break;
						}
					}
				}
				if (PQendcopy(pgsql)) {
					PHP_PQ_ERROR("endcopy failed: %s", pgsql);
					RETURN_FALSE;
				}
#endif
				while ((pgsql_result = PQgetResult(pgsql))) {
					PQclear(pgsql_result);
				}
			} else {
				PQclear(pgsql_result);
				RETURN_FALSE;
			}
			break;
		default:
			PQclear(pgsql_result);
			PHP_PQ_ERROR("Copy command failed: %s", pgsql);
			RETURN_FALSE;
			break;
	}
}
/* }}} */

/* {{{ proto bool pg_copy_from(resource connection, string table_name , array rows [, string delimiter [, string null_as]])
   Copy table from array */
PHP_FUNCTION(pg_copy_from)
{
	zval *pgsql_link = NULL, *pg_rows;
	zval *tmp;
	char *table_name, *pg_delim = NULL, *pg_null_as = NULL;
	size_t  table_name_len, pg_delim_len, pg_null_as_len;
	int  pg_null_as_free = 0;
	char *query;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;
	ExecStatusType status;
	int argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc, "rsa|ss",
							  &pgsql_link, &table_name, &table_name_len, &pg_rows,
							  &pg_delim, &pg_delim_len, &pg_null_as, &pg_null_as_len) == FAILURE) {
		return;
	}
	if (!pg_delim) {
		pg_delim = "\t";
	}
	if (!pg_null_as) {
		pg_null_as = estrdup("\\\\N");
		pg_null_as_free = 1;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	spprintf(&query, 0, "COPY %s FROM STDIN DELIMITERS E'%c' WITH NULL AS E'%s'", table_name, *pg_delim, pg_null_as);
	while ((pgsql_result = PQgetResult(pgsql))) {
		PQclear(pgsql_result);
	}
	pgsql_result = PQexec(pgsql, query);

	if (pg_null_as_free) {
		efree(pg_null_as);
	}
	efree(query);

	if (pgsql_result) {
		status = PQresultStatus(pgsql_result);
	} else {
		status = (ExecStatusType) PQstatus(pgsql);
	}

	switch (status) {
		case PGRES_COPY_IN:
			if (pgsql_result) {
				int command_failed = 0;
				PQclear(pgsql_result);
#if HAVE_PQPUTCOPYDATA
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pg_rows), tmp) {
					convert_to_string_ex(tmp);
					query = (char *)emalloc(Z_STRLEN_P(tmp) + 2);
					strlcpy(query, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp) + 2);
					if(Z_STRLEN_P(tmp) > 0 && *(query + Z_STRLEN_P(tmp) - 1) != '\n') {
						strlcat(query, "\n", Z_STRLEN_P(tmp) + 2);
					}
					if (PQputCopyData(pgsql, query, (int)strlen(query)) != 1) {
						efree(query);
						PHP_PQ_ERROR("copy failed: %s", pgsql);
						RETURN_FALSE;
					}
					efree(query);
				} ZEND_HASH_FOREACH_END();
				if (PQputCopyEnd(pgsql, NULL) != 1) {
					PHP_PQ_ERROR("putcopyend failed: %s", pgsql);
					RETURN_FALSE;
				}
#else
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pg_rows), tmp) {
					convert_to_string_ex(tmp);
					query = (char *)emalloc(Z_STRLEN_P(tmp) + 2);
					strlcpy(query, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp) + 2);
					if(Z_STRLEN_P(tmp) > 0 && *(query + Z_STRLEN_P(tmp) - 1) != '\n') {
						strlcat(query, "\n", Z_STRLEN_P(tmp) + 2);
					}
					if (PQputline(pgsql, query)==EOF) {
						efree(query);
						PHP_PQ_ERROR("copy failed: %s", pgsql);
						RETURN_FALSE;
					}
					efree(query);
				} ZEND_HASH_FOREACH_END();
				if (PQputline(pgsql, "\\.\n") == EOF) {
					PHP_PQ_ERROR("putline failed: %s", pgsql);
					RETURN_FALSE;
				}
				if (PQendcopy(pgsql)) {
					PHP_PQ_ERROR("endcopy failed: %s", pgsql);
					RETURN_FALSE;
				}
#endif
				while ((pgsql_result = PQgetResult(pgsql))) {
					if (PGRES_COMMAND_OK != PQresultStatus(pgsql_result)) {
						PHP_PQ_ERROR("Copy command failed: %s", pgsql);
						command_failed = 1;
					}
					PQclear(pgsql_result);
				}
				if (command_failed) {
					RETURN_FALSE;
				}
			} else {
				PQclear(pgsql_result);
				RETURN_FALSE;
			}
			RETURN_TRUE;
			break;
		default:
			PQclear(pgsql_result);
			PHP_PQ_ERROR("Copy command failed: %s", pgsql);
			RETURN_FALSE;
			break;
	}
}
/* }}} */

#ifdef HAVE_PQESCAPE
/* {{{ proto string pg_escape_string([resource connection,] string data)
   Escape string for text/char type */
PHP_FUNCTION(pg_escape_string)
{
	zend_string *from = NULL, *to = NULL;
	zval *pgsql_link;
#ifdef HAVE_PQESCAPE_CONN
	PGconn *pgsql;
#endif
	int id = -1;

	switch (ZEND_NUM_ARGS()) {
		case 1:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &from) == FAILURE) {
				return;
			}
			pgsql_link = NULL;
			id = FETCH_DEFAULT_LINK();
			break;

		default:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "rS", &pgsql_link, &from) == FAILURE) {
				return;
			}
			break;
	}

	to = zend_string_alloc(from->len * 2, 0);
#ifdef HAVE_PQESCAPE_CONN
	if (pgsql_link != NULL || id != -1) {
		ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
		to->len = PQescapeStringConn(pgsql, to->val, from->val, from->len, NULL);
	} else
#endif
	{
		to->len = PQescapeString(to->val, from->val, from->len);
	}

	to = zend_string_realloc(to, to->len, 0);
	RETURN_STR(to);
}
/* }}} */

/* {{{ proto string pg_escape_bytea([resource connection,] string data)
   Escape binary for bytea type  */
PHP_FUNCTION(pg_escape_bytea)
{
	char *from = NULL, *to = NULL;
	size_t to_len;
	size_t from_len;
	int id = -1;
#ifdef HAVE_PQESCAPE_BYTEA_CONN
	PGconn *pgsql;
#endif
	zval *pgsql_link;

	switch (ZEND_NUM_ARGS()) {
		case 1:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &from, &from_len) == FAILURE) {
				return;
			}
			pgsql_link = NULL;
			id = FETCH_DEFAULT_LINK();
			break;

		default:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &pgsql_link, &from, &from_len) == FAILURE) {
				return;
			}
			break;
	}

#ifdef HAVE_PQESCAPE_BYTEA_CONN
	if (pgsql_link != NULL || id != -1) {
		ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
		to = (char *)PQescapeByteaConn(pgsql, (unsigned char *)from, (size_t)from_len, &to_len);
	} else
#endif
		to = (char *)PQescapeBytea((unsigned char*)from, from_len, &to_len);

	RETVAL_STRINGL(to, to_len-1); /* to_len includes additional '\0' */
}
/* }}} */

#if !HAVE_PQUNESCAPEBYTEA
/* PQunescapeBytea() from PostgreSQL 7.3 to provide bytea unescape feature to 7.2 users.
   Renamed to php_pgsql_unescape_bytea() */
/*
 *		PQunescapeBytea - converts the null terminated string representation
 *		of a bytea, strtext, into binary, filling a buffer. It returns a
 *		pointer to the buffer which is NULL on error, and the size of the
 *		buffer in retbuflen. The pointer may subsequently be used as an
 *		argument to the function free(3). It is the reverse of PQescapeBytea.
 *
 *		The following transformations are reversed:
 *		'\0' == ASCII  0 == \000
 *		'\'' == ASCII 39 == \'
 *		'\\' == ASCII 92 == \\
 *
 *		States:
 *		0	normal		0->1->2->3->4
 *		1	\			   1->5
 *		2	\0			   1->6
 *		3	\00
 *		4	\000
 *		5	\'
 *		6	\\
 */
static unsigned char * php_pgsql_unescape_bytea(unsigned char *strtext, size_t *retbuflen) /* {{{ */
{
	size_t     buflen;
	unsigned char *buffer,
			   *sp,
			   *bp;
	unsigned int state = 0;

	if (strtext == NULL)
		return NULL;
	buflen = strlen(strtext);	/* will shrink, also we discover if
								 * strtext */
	buffer = (unsigned char *) emalloc(buflen);	/* isn't NULL terminated */
	for (bp = buffer, sp = strtext; *sp != '\0'; bp++, sp++)
	{
		switch (state)
		{
			case 0:
				if (*sp == '\\')
					state = 1;
				*bp = *sp;
				break;
			case 1:
				if (*sp == '\'')	/* state=5 */
				{				/* replace \' with 39 */
					bp--;
					*bp = '\'';
					buflen--;
					state = 0;
				}
				else if (*sp == '\\')	/* state=6 */
				{				/* replace \\ with 92 */
					bp--;
					*bp = '\\';
					buflen--;
					state = 0;
				}
				else
				{
					if (isdigit(*sp))
						state = 2;
					else
						state = 0;
					*bp = *sp;
				}
				break;
			case 2:
				if (isdigit(*sp))
					state = 3;
				else
					state = 0;
				*bp = *sp;
				break;
			case 3:
				if (isdigit(*sp))		/* state=4 */
				{
					unsigned char *start, *end, buf[4]; /* 000 + '\0' */
					
					bp -= 3;
					memcpy(buf, sp-2, 3);
					buf[3] = '\0';
					start = buf;
					*bp = (unsigned char)strtoul(start, (char **)&end, 8);
					buflen -= 3;
					state = 0;
				}
				else
				{
					*bp = *sp;
					state = 0;
				}
				break;
		}
	}
	buffer = erealloc(buffer, buflen+1);
	buffer[buflen] = '\0';

	*retbuflen = buflen;
	return buffer;
}
/* }}} */
#endif

/* {{{ proto string pg_unescape_bytea(string data)
   Unescape binary for bytea type  */
PHP_FUNCTION(pg_unescape_bytea)
{
	char *from = NULL, *to = NULL, *tmp = NULL;
	size_t to_len;
	size_t from_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s",
							  &from, &from_len) == FAILURE) {
		return;
	}

#if HAVE_PQUNESCAPEBYTEA
	tmp = (char *)PQunescapeBytea((unsigned char*)from, &to_len);
	to = estrndup(tmp, to_len);
	PQfreemem(tmp);
#else
	to = (char *)php_pgsql_unescape_bytea((unsigned char*)from, &to_len);
#endif
	if (!to) {
		php_error_docref(NULL, E_WARNING,"Invalid parameter");
		RETURN_FALSE;
	}
	RETVAL_STRINGL(to, to_len);
	efree(to);
}
/* }}} */
#endif

#ifdef HAVE_PQESCAPE
static void php_pgsql_escape_internal(INTERNAL_FUNCTION_PARAMETERS, int escape_literal) /* {{{ */ {
	char *from = NULL;
	zval *pgsql_link = NULL;
	PGconn *pgsql;
	size_t from_len;
	int id = -1;
	char *tmp;

	switch (ZEND_NUM_ARGS()) {
		case 1:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &from, &from_len) == FAILURE) {
				return;
			}
			pgsql_link = NULL;
			id = FETCH_DEFAULT_LINK();
			break;

		default:
			if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &pgsql_link, &from, &from_len) == FAILURE) {
				return;
			}
			break;
	}

	if (pgsql_link == NULL && id == -1) {
		php_error_docref(NULL, E_WARNING,"Cannot get default pgsql link");
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	if (pgsql == NULL) {
		php_error_docref(NULL, E_WARNING,"Cannot get pgsql link");
		RETURN_FALSE;
	}

	if (escape_literal) {
		tmp = PGSQLescapeLiteral(pgsql, from, (size_t)from_len);
	} else {
		tmp = PGSQLescapeIdentifier(pgsql, from, (size_t)from_len);
	}
	if (!tmp) {
		php_error_docref(NULL, E_WARNING,"Failed to escape");
		RETURN_FALSE;
	}

	RETVAL_STRING(tmp);
	PGSQLfree(tmp);
}
/* }}} */

/* {{{ proto string pg_escape_literal([resource connection,] string data)
   Escape parameter as string literal (i.e. parameter)	*/
PHP_FUNCTION(pg_escape_literal)
{
	php_pgsql_escape_internal(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */

/* {{{ proto string pg_escape_identifier([resource connection,] string data)
   Escape identifier (i.e. table name, field name)	*/
PHP_FUNCTION(pg_escape_identifier)
{
	php_pgsql_escape_internal(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */
#endif

/* {{{ proto string pg_result_error(resource result)
   Get error message associated with result */
PHP_FUNCTION(pg_result_error)
{
	zval *result;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	char *err = NULL;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r",
								 &result) == FAILURE) {
		RETURN_FALSE;
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	if (!pgsql_result) {
		RETURN_FALSE;
	}
	err = (char *)PQresultErrorMessage(pgsql_result);
	RETURN_STRING(err);
}
/* }}} */

#if HAVE_PQRESULTERRORFIELD
/* {{{ proto string pg_result_error_field(resource result, int fieldcode)
   Get error message field associated with result */
PHP_FUNCTION(pg_result_error_field)
{
	zval *result;
	zend_long fieldcode;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;
	char *field = NULL;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "rl",
								 &result, &fieldcode) == FAILURE) {
		RETURN_FALSE;
	}
	
	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	if (!pgsql_result) {
		RETURN_FALSE;
	}
	if (fieldcode & (PG_DIAG_SEVERITY|PG_DIAG_SQLSTATE|PG_DIAG_MESSAGE_PRIMARY|PG_DIAG_MESSAGE_DETAIL
				|PG_DIAG_MESSAGE_HINT|PG_DIAG_STATEMENT_POSITION
#if PG_DIAG_INTERNAL_POSITION
				|PG_DIAG_INTERNAL_POSITION
#endif
#if PG_DIAG_INTERNAL_QUERY
				|PG_DIAG_INTERNAL_QUERY
#endif
				|PG_DIAG_CONTEXT|PG_DIAG_SOURCE_FILE|PG_DIAG_SOURCE_LINE
				|PG_DIAG_SOURCE_FUNCTION)) {
		field = (char *)PQresultErrorField(pgsql_result, (int)fieldcode);
		if (field == NULL) {
			RETURN_NULL();
		} else {
			RETURN_STRING(field);
		}
	} else {
		RETURN_FALSE;
	}
}
/* }}} */
#endif

/* {{{ proto int pg_connection_status(resource connection)
   Get connection status */
PHP_FUNCTION(pg_connection_status)
{
	zval *pgsql_link = NULL;
	int id = -1;
	PGconn *pgsql;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	RETURN_LONG(PQstatus(pgsql));
}

/* }}} */

#if HAVE_PGTRANSACTIONSTATUS
/* {{{ proto int pg_transaction_status(resource connection)
   Get transaction status */
PHP_FUNCTION(pg_transaction_status)
{
	zval *pgsql_link = NULL;
	int id = -1;
	PGconn *pgsql;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	RETURN_LONG(PQtransactionStatus(pgsql));
}
#endif

/* }}} */

/* {{{ proto bool pg_connection_reset(resource connection)
   Reset connection (reconnect) */
PHP_FUNCTION(pg_connection_reset)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	PQreset(pgsql);
	if (PQstatus(pgsql) == CONNECTION_BAD) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} */

#define PHP_PG_ASYNC_IS_BUSY		1
#define PHP_PG_ASYNC_REQUEST_CANCEL 2

/* {{{ php_pgsql_flush_query
 */
static int php_pgsql_flush_query(PGconn *pgsql) 
{
	PGresult *res;
	int leftover = 0;
	
	if (PQ_SETNONBLOCKING(pgsql, 1)) {
		php_error_docref(NULL, E_NOTICE,"Cannot set connection to nonblocking mode");
		return -1;
	}
	while ((res = PQgetResult(pgsql))) {
		PQclear(res);
		leftover++;
	}
	PQ_SETNONBLOCKING(pgsql, 0);
	return leftover;
}
/* }}} */

/* {{{ php_pgsql_do_async
 */
static void php_pgsql_do_async(INTERNAL_FUNCTION_PARAMETERS, int entry_type) 
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (PQ_SETNONBLOCKING(pgsql, 1)) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}
	switch(entry_type) {
		case PHP_PG_ASYNC_IS_BUSY:
			PQconsumeInput(pgsql);
			RETVAL_LONG(PQisBusy(pgsql));
			break;
		case PHP_PG_ASYNC_REQUEST_CANCEL:
			RETVAL_LONG(PQrequestCancel(pgsql));
			while ((pgsql_result = PQgetResult(pgsql))) {
				PQclear(pgsql_result);
			}
			break;
		default:
			php_error_docref(NULL, E_ERROR, "PostgreSQL module error, please report this error");
			break;
	}
	if (PQ_SETNONBLOCKING(pgsql, 0)) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to blocking mode");
	}
	convert_to_boolean_ex(return_value);
}
/* }}} */

/* {{{ proto bool pg_cancel_query(resource connection)
   Cancel request */
PHP_FUNCTION(pg_cancel_query)
{
	php_pgsql_do_async(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_PG_ASYNC_REQUEST_CANCEL);
}
/* }}} */

/* {{{ proto bool pg_connection_busy(resource connection)
   Get connection is busy or not */
PHP_FUNCTION(pg_connection_busy)
{
	php_pgsql_do_async(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_PG_ASYNC_IS_BUSY);
}
/* }}} */

static int _php_pgsql_link_has_results(PGconn *pgsql) /* {{{ */
{
	PGresult *result;
	while ((result = PQgetResult(pgsql))) {
		PQclear(result);
		return 1;
	}
	return 0;
}
/* }}} */

/* {{{ proto bool pg_send_query(resource connection, string query)
   Send asynchronous query */
PHP_FUNCTION(pg_send_query)
{
	zval *pgsql_link;
	char *query;
	size_t len;
	int id = -1;
	PGconn *pgsql;
	int is_non_blocking;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rs", &pgsql_link, &query, &len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	is_non_blocking = PQisnonblocking(pgsql);

	if (is_non_blocking == 0 && PQ_SETNONBLOCKING(pgsql, 1) == -1) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}

	if (_php_pgsql_link_has_results(pgsql)) {
		php_error_docref(NULL, E_NOTICE,
			"There are results on this connection. Call pg_get_result() until it returns FALSE");
	}

	if (is_non_blocking) {
		if (!PQsendQuery(pgsql, query)) {
			RETURN_FALSE;
		}
		ret = PQflush(pgsql);
	} else {
		if (!PQsendQuery(pgsql, query)) {
			if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
				PQreset(pgsql);
			}
			if (!PQsendQuery(pgsql, query)) {
				RETURN_FALSE;
			}
		}

		/* Wait to finish sending buffer */
		while ((ret = PQflush(pgsql))) {
			if (ret == -1) {
				php_error_docref(NULL, E_NOTICE, "Could not empty PostgreSQL send buffer");
				break;
			}
			usleep(10000);
		}

		if (PQ_SETNONBLOCKING(pgsql, 0)) {
			php_error_docref(NULL, E_NOTICE, "Cannot set connection to blocking mode");
		}
	}

	if (ret == 0) {
		RETURN_TRUE;
	} else if (ret == -1) {
		RETURN_FALSE;
	} else {
		RETURN_LONG(0);
	}
}
/* }}} */

#if HAVE_PQSENDQUERYPARAMS
/* {{{ proto bool pg_send_query_params(resource connection, string query, array params)
   Send asynchronous parameterized query */
PHP_FUNCTION(pg_send_query_params)
{
	zval *pgsql_link, *pv_param_arr, *tmp;
	int num_params = 0;
	char **params = NULL;
	char *query;
	size_t query_len;
	int id = -1;
	PGconn *pgsql;
	int is_non_blocking;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rsa/", &pgsql_link, &query, &query_len, &pv_param_arr) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	is_non_blocking = PQisnonblocking(pgsql);

	if (is_non_blocking == 0 && PQ_SETNONBLOCKING(pgsql, 1) == -1) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}

	if (_php_pgsql_link_has_results(pgsql)) {
		php_error_docref(NULL, E_NOTICE,
			"There are results on this connection. Call pg_get_result() until it returns FALSE");
	}

	num_params = zend_hash_num_elements(Z_ARRVAL_P(pv_param_arr));
	if (num_params > 0) {
		int i = 0;
		params = (char **)safe_emalloc(sizeof(char *), num_params, 0);
		
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pv_param_arr), tmp) {

			if (Z_TYPE_P(tmp) == IS_NULL) {
				params[i] = NULL;
			} else {
				zval tmp_val;
				ZVAL_COPY(&tmp_val, tmp);
				convert_to_string(&tmp_val);
				if (Z_TYPE(tmp_val) != IS_STRING) {
					php_error_docref(NULL, E_WARNING,"Error converting parameter");
					zval_ptr_dtor(&tmp_val);
					_php_pgsql_free_params(params, num_params);
					RETURN_FALSE;
				}
				params[i] = estrndup(Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
				zval_ptr_dtor(&tmp_val);
			}

			i++;
		} ZEND_HASH_FOREACH_END();
	}

	if (PQsendQueryParams(pgsql, query, num_params, NULL, (const char * const *)params, NULL, NULL, 0)) {
		_php_pgsql_free_params(params, num_params);
	} else if (is_non_blocking) {
		_php_pgsql_free_params(params, num_params);
		RETURN_FALSE;
	} else {
		if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
			PQreset(pgsql);
		}
		if (!PQsendQueryParams(pgsql, query, num_params, NULL, (const char * const *)params, NULL, NULL, 0)) {
			_php_pgsql_free_params(params, num_params);
			RETURN_FALSE;
		}
	}

	if (is_non_blocking) {
		ret = PQflush(pgsql);
	} else {
		/* Wait to finish sending buffer */
		while ((ret = PQflush(pgsql))) {
			if (ret == -1) {
				php_error_docref(NULL, E_NOTICE, "Could not empty PostgreSQL send buffer");
				break;
			}
			usleep(10000);
		}

		if (PQ_SETNONBLOCKING(pgsql, 0) != 0) {
			php_error_docref(NULL, E_NOTICE, "Cannot set connection to blocking mode");
		}
	}

	if (ret == 0) {
		RETURN_TRUE;
	} else if (ret == -1) {
		RETURN_FALSE;
	} else {
		RETURN_LONG(0);
	}
}
/* }}} */
#endif

#if HAVE_PQSENDPREPARE
/* {{{ proto bool pg_send_prepare(resource connection, string stmtname, string query)
   Asynchronously prepare a query for future execution */
PHP_FUNCTION(pg_send_prepare)
{
	zval *pgsql_link;
	char *query, *stmtname;
	size_t stmtname_len, query_len;
	int id = -1;
	PGconn *pgsql;
	int is_non_blocking;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rss", &pgsql_link, &stmtname, &stmtname_len, &query, &query_len) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	is_non_blocking = PQisnonblocking(pgsql);

	if (is_non_blocking == 0 && PQ_SETNONBLOCKING(pgsql, 1) == -1) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}

	if (_php_pgsql_link_has_results(pgsql)) {
		php_error_docref(NULL, E_NOTICE,
			"There are results on this connection. Call pg_get_result() until it returns FALSE");
	}

	if (!PQsendPrepare(pgsql, stmtname, query, 0, NULL)) {
		if (is_non_blocking) {
			RETURN_FALSE;
		} else {
			if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
				PQreset(pgsql);
			}
			if (!PQsendPrepare(pgsql, stmtname, query, 0, NULL)) {
				RETURN_FALSE;
			}
		}
	}

	if (is_non_blocking) {
		ret = PQflush(pgsql);
	} else {
		/* Wait to finish sending buffer */
		while ((ret = PQflush(pgsql))) {
			if (ret == -1) {
				php_error_docref(NULL, E_NOTICE, "Could not empty PostgreSQL send buffer");
				break;
			}
			usleep(10000);
		}
		if (PQ_SETNONBLOCKING(pgsql, 0) != 0) {
			php_error_docref(NULL, E_NOTICE, "Cannot set connection to blocking mode");
		}
	}

	if (ret == 0) {
		RETURN_TRUE;
	} else if (ret == -1) {
		RETURN_FALSE;
	} else {
		RETURN_LONG(0);
	}
}
/* }}} */
#endif

#if HAVE_PQSENDQUERYPREPARED
/* {{{ proto bool pg_send_execute(resource connection, string stmtname, array params)
   Executes prevriously prepared stmtname asynchronously */
PHP_FUNCTION(pg_send_execute)
{
	zval *pgsql_link;
	zval *pv_param_arr, *tmp;
	int num_params = 0;
	char **params = NULL;
	char *stmtname;
	size_t stmtname_len;
	int id = -1;
	PGconn *pgsql;
	int is_non_blocking;
	int ret;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "rsa", &pgsql_link, &stmtname, &stmtname_len, &pv_param_arr) == FAILURE) {
		return;
	}

	if (pgsql_link == NULL) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	is_non_blocking = PQisnonblocking(pgsql);

	if (is_non_blocking == 0 && PQ_SETNONBLOCKING(pgsql, 1) == -1) {
		php_error_docref(NULL, E_NOTICE, "Cannot set connection to nonblocking mode");
		RETURN_FALSE;
	}

	if (_php_pgsql_link_has_results(pgsql)) {
		php_error_docref(NULL, E_NOTICE,
			"There are results on this connection. Call pg_get_result() until it returns FALSE");
	}

	num_params = zend_hash_num_elements(Z_ARRVAL_P(pv_param_arr));
	if (num_params > 0) {
		int i = 0;
		params = (char **)safe_emalloc(sizeof(char *), num_params, 0);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(pv_param_arr), tmp) {

			if (Z_TYPE_P(tmp) == IS_NULL) {
				params[i] = NULL;
			} else {
				zval tmp_val;
				ZVAL_COPY(&tmp_val, tmp);
				convert_to_string(&tmp_val);
				if (Z_TYPE(tmp_val) != IS_STRING) {
					php_error_docref(NULL, E_WARNING,"Error converting parameter");
					zval_ptr_dtor(&tmp_val);
					_php_pgsql_free_params(params, num_params);
					RETURN_FALSE;
				}
				params[i] = estrndup(Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
				zval_ptr_dtor(&tmp_val);
			}

			i++;
		} ZEND_HASH_FOREACH_END();
	}

	if (PQsendQueryPrepared(pgsql, stmtname, num_params, (const char * const *)params, NULL, NULL, 0)) {
		_php_pgsql_free_params(params, num_params);
	} else if (is_non_blocking) {
		_php_pgsql_free_params(params, num_params);
		RETURN_FALSE;
	} else {
		if ((PGG(auto_reset_persistent) & 2) && PQstatus(pgsql) != CONNECTION_OK) {
			PQreset(pgsql);
		}
		if (!PQsendQueryPrepared(pgsql, stmtname, num_params, (const char * const *)params, NULL, NULL, 0)) {
			_php_pgsql_free_params(params, num_params);
			RETURN_FALSE;
		}
	}

	if (is_non_blocking) {
		ret = PQflush(pgsql);
	} else {
		/* Wait to finish sending buffer */
		while ((ret = PQflush(pgsql))) {
			if (ret == -1) {
				php_error_docref(NULL, E_NOTICE, "Could not empty PostgreSQL send buffer");
				break;
			}
			usleep(10000);
		}
		if (PQ_SETNONBLOCKING(pgsql, 0) != 0) {
			php_error_docref(NULL, E_NOTICE, "Cannot set connection to blocking mode");
		}
	}

	if (ret == 0) {
		RETURN_TRUE;
	} else if (ret == -1) {
		RETURN_FALSE;
	} else {
		RETURN_LONG(0);
	}
}
/* }}} */
#endif

/* {{{ proto resource pg_get_result(resource connection)
   Get asynchronous query result */
PHP_FUNCTION(pg_get_result)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r", &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	pgsql_result = PQgetResult(pgsql);
	if (!pgsql_result) {
		/* no result */
		RETURN_FALSE;
	}
	pg_result = (pgsql_result_handle *) emalloc(sizeof(pgsql_result_handle));
	pg_result->conn = pgsql;
	pg_result->result = pgsql_result;
	pg_result->row = 0;
	ZEND_REGISTER_RESOURCE(return_value, pg_result, le_result);
}
/* }}} */

/* {{{ proto mixed pg_result_status(resource result[, long result_type])
   Get status of query result */
PHP_FUNCTION(pg_result_status)
{
	zval *result;
	zend_long result_type = PGSQL_STATUS_LONG;
	ExecStatusType status;
	PGresult *pgsql_result;
	pgsql_result_handle *pg_result;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r|l",
								 &result, &result_type) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE(pg_result, pgsql_result_handle *, result, -1, "PostgreSQL result", le_result);

	pgsql_result = pg_result->result;
	if (result_type == PGSQL_STATUS_LONG) {
		status = PQresultStatus(pgsql_result);
		RETURN_LONG((int)status);
	}
	else if (result_type == PGSQL_STATUS_STRING) {
		RETURN_STRING(PQcmdStatus(pgsql_result));
	}
	else {
		php_error_docref(NULL, E_WARNING, "Optional 2nd parameter should be PGSQL_STATUS_LONG or PGSQL_STATUS_STRING");
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto array pg_get_notify([resource connection[, result_type]])
   Get asynchronous notification */
PHP_FUNCTION(pg_get_notify)
{
	zval *pgsql_link;
	int id = -1;
	zend_long result_type = PGSQL_ASSOC;
	PGconn *pgsql;
	PGnotify *pgsql_notify;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "r|l",
								 &pgsql_link, &result_type) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (!(result_type & PGSQL_BOTH)) {
		php_error_docref(NULL, E_WARNING, "Invalid result type");
		RETURN_FALSE;
	}

	PQconsumeInput(pgsql);
	pgsql_notify = PQnotifies(pgsql);
	if (!pgsql_notify) {
		/* no notify message */
		RETURN_FALSE;
	}
	array_init(return_value);
	if (result_type & PGSQL_NUM) {
		add_index_string(return_value, 0, pgsql_notify->relname);
		add_index_long(return_value, 1, pgsql_notify->be_pid);
#if HAVE_PQPROTOCOLVERSION && HAVE_PQPARAMETERSTATUS 
		if (PQprotocolVersion(pgsql) >= 3 && atof(PQparameterStatus(pgsql, "server_version")) >= 9.0) {
#else 
		if (atof(PG_VERSION) >= 9.0) {
#endif 
#if HAVE_PQPARAMETERSTATUS
			add_index_string(return_value, 2, pgsql_notify->extra);
#endif
		}
	}
	if (result_type & PGSQL_ASSOC) {
		add_assoc_string(return_value, "message", pgsql_notify->relname);
		add_assoc_long(return_value, "pid", pgsql_notify->be_pid);
#if HAVE_PQPROTOCOLVERSION && HAVE_PQPARAMETERSTATUS 
		if (PQprotocolVersion(pgsql) >= 3 && atof(PQparameterStatus(pgsql, "server_version")) >= 9.0) {
#else 
		if (atof(PG_VERSION) >= 9.0) {
#endif 
#if HAVE_PQPARAMETERSTATUS
			add_assoc_string(return_value, "payload", pgsql_notify->extra);
#endif
		}
	}
	PQfreemem(pgsql_notify);
}
/* }}} */

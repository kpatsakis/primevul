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

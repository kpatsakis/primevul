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

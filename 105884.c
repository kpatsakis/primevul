PHP_FUNCTION(pg_lo_create)
{
	zval *pgsql_link = NULL, *oid = NULL;
	PGconn *pgsql;
	Oid pgsql_oid, wanted_oid = InvalidOid;
	int id = -1, argc = ZEND_NUM_ARGS();

	if (zend_parse_parameters(argc TSRMLS_CC, "|zz", &pgsql_link, &oid) == FAILURE) {
		return;
	}

	if ((argc == 1) && (Z_TYPE_P(pgsql_link) != IS_RESOURCE)) {
		oid = pgsql_link;
		pgsql_link = NULL;
	}
	
	if (pgsql_link == NULL) {
		id = PGG(default_link);
		CHECK_DEFAULT_LINK(id);
		if (id == -1) {
			RETURN_FALSE;
		}
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	if (oid) {
#ifndef HAVE_PG_LO_CREATE
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Passing OID value is not supported. Upgrade your PostgreSQL");
#else
		switch (Z_TYPE_P(oid)) {
		case IS_STRING:
			{
				char *end_ptr;
				wanted_oid = (Oid)strtoul(Z_STRVAL_P(oid), &end_ptr, 10);
				if ((Z_STRVAL_P(oid)+Z_STRLEN_P(oid)) != end_ptr) {
				/* wrong integer format */
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "invalid OID value passed");
				RETURN_FALSE;
				}
			}
			break;
		case IS_LONG:
			if (Z_LVAL_P(oid) < (long)InvalidOid) {
				php_error_docref(NULL TSRMLS_CC, E_NOTICE, "invalid OID value passed");
				RETURN_FALSE;
			}
			wanted_oid = (Oid)Z_LVAL_P(oid);
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "invalid OID value passed");
			RETURN_FALSE;
        }
		if ((pgsql_oid = lo_create(pgsql, wanted_oid)) == InvalidOid) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to create PostgreSQL large object");
			RETURN_FALSE;
		}

		PGSQL_RETURN_OID(pgsql_oid);
#endif
	}

	if ((pgsql_oid = lo_creat(pgsql, INV_READ|INV_WRITE)) == InvalidOid) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to create PostgreSQL large object");
		RETURN_FALSE;
	}

	PGSQL_RETURN_OID(pgsql_oid);
}

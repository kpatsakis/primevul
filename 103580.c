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

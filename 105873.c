PHP_FUNCTION(pg_last_notice) 
{
	zval *pgsql_link;
	PGconn *pg_link;
	int id = -1;
	php_pgsql_notice **notice;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &pgsql_link) == FAILURE) {
		return;
	}
	/* Just to check if user passed valid resoruce */
	ZEND_FETCH_RESOURCE2(pg_link, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (zend_hash_index_find(&PGG(notices), Z_RESVAL_P(pgsql_link), (void **)&notice) == FAILURE) {
		RETURN_FALSE;
	}
	RETURN_STRINGL((*notice)->message, (*notice)->len, 1);
}

static PHP_GINIT_FUNCTION(pgsql)
{
#if defined(COMPILE_DL_PGSQL) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE;
#endif
	memset(pgsql_globals, 0, sizeof(zend_pgsql_globals));
	/* Initilize notice message hash at MINIT only */
	zend_hash_init_ex(&pgsql_globals->notices, 0, NULL, PHP_PGSQL_NOTICE_PTR_DTOR, 1, 0); 
}

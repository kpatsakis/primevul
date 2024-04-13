static PHP_GINIT_FUNCTION(pgsql)
{
	memset(pgsql_globals, 0, sizeof(zend_pgsql_globals));
	/* Initilize notice message hash at MINIT only */
	zend_hash_init_ex(&pgsql_globals->notices, 0, NULL, PHP_PGSQL_NOTICE_PTR_DTOR, 1, 0); 
}

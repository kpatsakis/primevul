static void _php_pgsql_notice_handler(void *resource_id, const char *message)
{
	php_pgsql_notice *notice;
	
	if (! PGG(ignore_notices)) {
		notice = (php_pgsql_notice *)emalloc(sizeof(php_pgsql_notice));
		notice->message = _php_pgsql_trim_message(message, &notice->len);
		if (PGG(log_notices)) {
			php_error_docref(NULL, E_NOTICE, "%s", notice->message);
		}
		zend_hash_index_update_ptr(&PGG(notices), (zend_ulong)resource_id, notice);
	}
}

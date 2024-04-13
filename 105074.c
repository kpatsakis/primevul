static PHP_FUNCTION(session_write_close)
{
	php_session_flush(TSRMLS_C);
}

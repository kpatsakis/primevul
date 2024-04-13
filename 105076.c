static PHP_FUNCTION(session_register_shutdown)
{
	php_shutdown_function_entry shutdown_function_entry;
	zval *callback;

	/* This function is registered itself as a shutdown function by
	 * session_set_save_handler($obj). The reason we now register another
	 * shutdown function is in case the user registered their own shutdown
	 * function after calling session_set_save_handler(), which expects
	 * the session still to be available.
	 */

	shutdown_function_entry.arg_count = 1;
	shutdown_function_entry.arguments = (zval **) safe_emalloc(sizeof(zval *), 1, 0);

	MAKE_STD_ZVAL(callback);
	ZVAL_STRING(callback, "session_write_close", 1);
	shutdown_function_entry.arguments[0] = callback;

	if (!append_user_shutdown_function(shutdown_function_entry TSRMLS_CC)) {
		zval_ptr_dtor(&callback);
		efree(shutdown_function_entry.arguments);

		/* Unable to register shutdown function, presumably because of lack
		 * of memory, so flush the session now. It would be done in rshutdown
		 * anyway but the handler will have had it's dtor called by then.
		 * If the user does have a later shutdown function which needs the
		 * session then tough luck.
		 */
		php_session_flush(TSRMLS_C);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to register session flush function");
	}
}

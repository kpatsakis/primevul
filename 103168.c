static void run_user_tick_functions(int tick_count) /* {{{ */
{
	TSRMLS_FETCH();

	zend_llist_apply(BG(user_tick_functions), (llist_apply_func_t) user_tick_function_call TSRMLS_CC);
}
/* }}} */

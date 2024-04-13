PHP_FUNCTION(register_tick_function)
{
	user_tick_function_entry tick_fe;
	int i;
	char *function_name = NULL;

	tick_fe.calling = 0;
	tick_fe.arg_count = ZEND_NUM_ARGS();

	if (tick_fe.arg_count < 1) {
		WRONG_PARAM_COUNT;
	}

	tick_fe.arguments = (zval **) safe_emalloc(sizeof(zval *), tick_fe.arg_count, 0);

	if (zend_get_parameters_array(ht, tick_fe.arg_count, tick_fe.arguments) == FAILURE) {
		efree(tick_fe.arguments);
		RETURN_FALSE;
	}

	if (!zend_is_callable(tick_fe.arguments[0], 0, &function_name TSRMLS_CC)) {
		efree(tick_fe.arguments);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid tick callback '%s' passed", function_name);
		efree(function_name);
		RETURN_FALSE;
	} else if (function_name) {
		efree(function_name);
	}

	if (Z_TYPE_P(tick_fe.arguments[0]) != IS_ARRAY && Z_TYPE_P(tick_fe.arguments[0]) != IS_OBJECT) {
		convert_to_string_ex(&tick_fe.arguments[0]);
	}

	if (!BG(user_tick_functions)) {
		BG(user_tick_functions) = (zend_llist *) emalloc(sizeof(zend_llist));
		zend_llist_init(BG(user_tick_functions),
						sizeof(user_tick_function_entry),
						(llist_dtor_func_t) user_tick_function_dtor, 0);
		php_add_tick_function(run_user_tick_functions);
	}

	for (i = 0; i < tick_fe.arg_count; i++) {
		Z_ADDREF_P(tick_fe.arguments[i]);
	}

	zend_llist_add_element(BG(user_tick_functions), &tick_fe);

	RETURN_TRUE;
}

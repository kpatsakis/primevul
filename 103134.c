PHP_FUNCTION(unregister_tick_function)
{
	zval *function;
	user_tick_function_entry tick_fe;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/", &function) == FAILURE) {
		return;
	}

	if (!BG(user_tick_functions)) {
		return;
	}

	if (Z_TYPE_P(function) != IS_ARRAY && Z_TYPE_P(function) != IS_OBJECT) {
		convert_to_string(function);
	}

	tick_fe.arguments = (zval **) emalloc(sizeof(zval *));
	tick_fe.arguments[0] = function;
	tick_fe.arg_count = 1;
	zend_llist_del_element(BG(user_tick_functions), &tick_fe, (int (*)(void *, void *)) user_tick_function_compare);
	efree(tick_fe.arguments);
}

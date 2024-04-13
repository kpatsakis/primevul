PHP_FUNCTION(get_cfg_var)
{
	char *varname;
	int varname_len;
	zval *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &varname, &varname_len) == FAILURE) {
		return;
	}

	retval = cfg_get_entry(varname, varname_len + 1);

	if (retval) {
		if (Z_TYPE_P(retval) == IS_ARRAY) {
			array_init(return_value);
			zend_hash_apply_with_arguments(Z_ARRVAL_P(retval) TSRMLS_CC, (apply_func_args_t) add_config_entry_cb, 1, return_value);
			return;
		} else {
			RETURN_STRING(Z_STRVAL_P(retval), 1);
		}
	} else {
		RETURN_FALSE;
	}
}

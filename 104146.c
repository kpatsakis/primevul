static void cgi_php_load_env_var(char *var, unsigned int var_len, char *val, unsigned int val_len, void *arg TSRMLS_DC)
{
	zval *array_ptr = (zval*)arg;	
	int filter_arg = (array_ptr == PG(http_globals)[TRACK_VARS_ENV])?PARSE_ENV:PARSE_SERVER;
	unsigned int new_val_len;

	if (sapi_module.input_filter(filter_arg, var, &val, strlen(val), &new_val_len TSRMLS_CC)) {
		php_register_variable_safe(var, val, new_val_len, array_ptr TSRMLS_CC);
	}
}

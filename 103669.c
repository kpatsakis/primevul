static int php_pgsql_add_quotes(zval *src, zend_bool should_free) 
{
	smart_str str = {0};

	assert(Z_TYPE_P(src) == IS_STRING);
	assert(should_free == 1 || should_free == 0);

	smart_str_appendc(&str, 'E');
	smart_str_appendc(&str, '\'');
	smart_str_appendl(&str, Z_STRVAL_P(src), Z_STRLEN_P(src));
	smart_str_appendc(&str, '\'');
	smart_str_0(&str);

	if (should_free) {
		zval_ptr_dtor(src);
	}
	ZVAL_NEW_STR(src, str.s);

	return SUCCESS;
}

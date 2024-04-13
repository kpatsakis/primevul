PHP_FUNCTION(get_current_user)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_STRING(php_get_current_user(TSRMLS_C), 1);
}

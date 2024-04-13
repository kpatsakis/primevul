PHP_FUNCTION(get_magic_quotes_runtime)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_FALSE;
}

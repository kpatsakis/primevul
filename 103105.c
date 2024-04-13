PHP_FUNCTION(get_magic_quotes_gpc)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_FALSE;
}

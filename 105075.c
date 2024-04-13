static PHP_FUNCTION(session_status)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(PS(session_status));
}

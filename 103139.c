PHP_FUNCTION(sys_getloadavg)
{
	double load[3];

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (getloadavg(load, 3) == -1) {
		RETURN_FALSE;
	} else {
		array_init(return_value);
		add_index_double(return_value, 0, load[0]);
		add_index_double(return_value, 1, load[1]);
		add_index_double(return_value, 2, load[2]);
	}
}

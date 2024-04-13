static int php_ini_check_path(char *option_name, int option_len, char *new_option_name, int new_option_len) /* {{{ */
{
	if (option_len != (new_option_len - 1)) {
		return 0;
	}

	return !strncmp(option_name, new_option_name, option_len);
}
/* }}} */

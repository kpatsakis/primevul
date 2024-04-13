PHPAPI int php_session_register_module(ps_module *ptr) /* {{{ */
{
	int ret = -1;
	int i;

	for (i = 0; i < MAX_MODULES; i++) {
		if (!ps_modules[i]) {
			ps_modules[i] = ptr;
			ret = 0;
			break;
		}
	}
	return ret;
}
/* }}} */

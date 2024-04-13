static ZEND_INI_MH(OnUpdateSmartStr) /* {{{ */
{
	smart_str *p;
#ifndef ZTS
	char *base = (char *) mh_arg2;
#else
	char *base;

	base = (char *) ts_resource(*((int *) mh_arg2));
#endif

	p = (smart_str *) (base+(size_t) mh_arg1);

	smart_str_sets(p, new_value);

	return SUCCESS;
}
/* }}} */

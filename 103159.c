PHPAPI double php_get_inf(void) /* {{{ */
{
#if HAVE_HUGE_VAL_INF
	return HUGE_VAL;
#elif defined(__i386__) || defined(_X86_) || defined(ALPHA) || defined(_ALPHA) || defined(__alpha)
	double val = 0.0;
	((php_uint32*)&val)[1] = PHP_DOUBLE_INFINITY_HIGH;
	((php_uint32*)&val)[0] = 0;
	return val;
#elif HAVE_ATOF_ACCEPTS_INF
	return atof("INF");
#else
	return 1.0/0.0;
#endif
}
/* }}} */

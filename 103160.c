PHPAPI double php_get_nan(void) /* {{{ */
{
#if HAVE_HUGE_VAL_NAN
	return HUGE_VAL + -HUGE_VAL;
#elif defined(__i386__) || defined(_X86_) || defined(ALPHA) || defined(_ALPHA) || defined(__alpha)
	double val = 0.0;
	((php_uint32*)&val)[1] = PHP_DOUBLE_QUIET_NAN_HIGH;
	((php_uint32*)&val)[0] = 0;
	return val;
#elif HAVE_ATOF_ACCEPTS_NAN
	return atof("NAN");
#else
	return 0.0/0.0;
#endif
}
/* }}} */

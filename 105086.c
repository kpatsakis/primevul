static PHP_MINIT_FUNCTION(session) /* {{{ */
{
	zend_class_entry ce;

	zend_register_auto_global("_SESSION", sizeof("_SESSION")-1, 0, NULL TSRMLS_CC);

	PS(module_number) = module_number; /* if we really need this var we need to init it in zts mode as well! */

	PS(session_status) = php_session_none;
	REGISTER_INI_ENTRIES();

#ifdef HAVE_LIBMM
	PHP_MINIT(ps_mm) (INIT_FUNC_ARGS_PASSTHRU);
#endif
	php_session_rfc1867_orig_callback = php_rfc1867_callback;
	php_rfc1867_callback = php_session_rfc1867_callback;

	/* Register interfaces */
	INIT_CLASS_ENTRY(ce, PS_IFACE_NAME, php_session_iface_functions);
	php_session_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	php_session_iface_entry->ce_flags |= ZEND_ACC_INTERFACE;

	INIT_CLASS_ENTRY(ce, PS_SID_IFACE_NAME, php_session_id_iface_functions);
	php_session_id_iface_entry = zend_register_internal_class(&ce TSRMLS_CC);
	php_session_id_iface_entry->ce_flags |= ZEND_ACC_INTERFACE;

	/* Register base class */
	INIT_CLASS_ENTRY(ce, PS_CLASS_NAME, php_session_class_functions);
	php_session_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_class_implements(php_session_class_entry TSRMLS_CC, 1, php_session_iface_entry);
	zend_class_implements(php_session_class_entry TSRMLS_CC, 1, php_session_id_iface_entry);

	REGISTER_LONG_CONSTANT("PHP_SESSION_DISABLED", php_session_disabled, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PHP_SESSION_NONE", php_session_none, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PHP_SESSION_ACTIVE", php_session_active, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

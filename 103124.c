PHP_FUNCTION(restore_include_path)
{
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		return;
	}
	zend_restore_ini_entry("include_path", sizeof("include_path"), PHP_INI_STAGE_RUNTIME);
}

PHP_FUNCTION(ini_restore)
{
	char *varname;
	int varname_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &varname, &varname_len) == FAILURE) {
		return;
	}

	zend_restore_ini_entry(varname, varname_len+1, PHP_INI_STAGE_RUNTIME);
}

PHP_PHAR_API int phar_resolve_alias(char *alias, int alias_len, char **filename, int *filename_len TSRMLS_DC) /* {{{ */ {
	phar_archive_data **fd_ptr;
	if (PHAR_GLOBALS->phar_alias_map.arBuckets
			&& SUCCESS == zend_hash_find(&(PHAR_GLOBALS->phar_alias_map), alias, alias_len, (void**)&fd_ptr)) {
		*filename = (*fd_ptr)->fname;
		*filename_len = (*fd_ptr)->fname_len;
		return SUCCESS;
	}
	return FAILURE;
}
/* }}} */

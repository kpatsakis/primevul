int phar_copy_on_write(phar_archive_data **pphar TSRMLS_DC) /* {{{ */
{
	phar_archive_data **newpphar, *newphar = NULL;

	if (SUCCESS != zend_hash_add(&(PHAR_GLOBALS->phar_fname_map), (*pphar)->fname, (*pphar)->fname_len, (void *)&newphar, sizeof(phar_archive_data *), (void **)&newpphar)) {
		return FAILURE;
	}

	*newpphar = *pphar;
	phar_copy_cached_phar(newpphar TSRMLS_CC);
	/* invalidate phar cache */
	PHAR_G(last_phar) = NULL;
	PHAR_G(last_phar_name) = PHAR_G(last_alias) = NULL;

	if (newpphar[0]->alias_len && FAILURE == zend_hash_add(&(PHAR_GLOBALS->phar_alias_map), newpphar[0]->alias, newpphar[0]->alias_len, (void*)newpphar, sizeof(phar_archive_data*), NULL)) {
		zend_hash_del(&(PHAR_GLOBALS->phar_fname_map), (*pphar)->fname, (*pphar)->fname_len);
		return FAILURE;
	}

	*pphar = *newpphar;
	return SUCCESS;
}
/* }}} */

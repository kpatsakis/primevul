static void phar_copy_cached_phar(phar_archive_data **pphar TSRMLS_DC) /* {{{ */
{
	phar_archive_data *phar;
	HashTable newmanifest;
	char *fname;
	phar_archive_object **objphar;

	phar = (phar_archive_data *) emalloc(sizeof(phar_archive_data));
	*phar = **pphar;
	phar->is_persistent = 0;
	fname = phar->fname;
	phar->fname = estrndup(phar->fname, phar->fname_len);
	phar->ext = phar->fname + (phar->ext - fname);

	if (phar->alias) {
		phar->alias = estrndup(phar->alias, phar->alias_len);
	}

	if (phar->signature) {
		phar->signature = estrdup(phar->signature);
	}

	if (phar->metadata) {
		/* assume success, we would have failed before */
		if (phar->metadata_len) {
			char *buf = estrndup((char *) phar->metadata, phar->metadata_len);
			phar_parse_metadata(&buf, &phar->metadata, phar->metadata_len TSRMLS_CC);
			efree(buf);
		} else {
			zval *t;

			t = phar->metadata;
			ALLOC_ZVAL(phar->metadata);
			*phar->metadata = *t;
			zval_copy_ctor(phar->metadata);
			Z_SET_REFCOUNT_P(phar->metadata, 1);
		}
	}

	zend_hash_init(&newmanifest, sizeof(phar_entry_info),
		zend_get_hash_value, destroy_phar_manifest_entry, 0);
	zend_hash_copy(&newmanifest, &(*pphar)->manifest, NULL, NULL, sizeof(phar_entry_info));
	zend_hash_apply_with_argument(&newmanifest, (apply_func_arg_t) phar_update_cached_entry, (void *)phar TSRMLS_CC);
	phar->manifest = newmanifest;
	zend_hash_init(&phar->mounted_dirs, sizeof(char *),
		zend_get_hash_value, NULL, 0);
	zend_hash_init(&phar->virtual_dirs, sizeof(char *),
		zend_get_hash_value, NULL, 0);
	zend_hash_copy(&phar->virtual_dirs, &(*pphar)->virtual_dirs, NULL, NULL, sizeof(void *));
	*pphar = phar;

	/* now, scan the list of persistent Phar objects referencing this phar and update the pointers */
	for (zend_hash_internal_pointer_reset(&PHAR_GLOBALS->phar_persist_map);
	SUCCESS == zend_hash_get_current_data(&PHAR_GLOBALS->phar_persist_map, (void **) &objphar);
	zend_hash_move_forward(&PHAR_GLOBALS->phar_persist_map)) {
		if (objphar[0]->arc.archive->fname_len == phar->fname_len && !memcmp(objphar[0]->arc.archive->fname, phar->fname, phar->fname_len)) {
			objphar[0]->arc.archive = phar;
		}
	}
}
/* }}} */

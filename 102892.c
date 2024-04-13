static int phar_update_cached_entry(void *data, void *argument) /* {{{ */
{
	phar_entry_info *entry = (phar_entry_info *)data;
	TSRMLS_FETCH();

	entry->phar = (phar_archive_data *)argument;

	if (entry->link) {
		entry->link = estrdup(entry->link);
	}

	if (entry->tmp) {
		entry->tmp = estrdup(entry->tmp);
	}

	entry->metadata_str.c = 0;
	entry->filename = estrndup(entry->filename, entry->filename_len);
	entry->is_persistent = 0;

	if (entry->metadata) {
		if (entry->metadata_len) {
			char *buf = estrndup((char *) entry->metadata, entry->metadata_len);
			/* assume success, we would have failed before */
			phar_parse_metadata((char **) &buf, &entry->metadata, entry->metadata_len TSRMLS_CC);
			efree(buf);
		} else {
			zval *t;

			t = entry->metadata;
			ALLOC_ZVAL(entry->metadata);
			*entry->metadata = *t;
			zval_copy_ctor(entry->metadata);
			Z_SET_REFCOUNT_P(entry->metadata, 1);
			entry->metadata_str.c = NULL;
			entry->metadata_str.len = 0;
		}
	}
	return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

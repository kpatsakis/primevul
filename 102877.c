php_stream *phar_get_efp(phar_entry_info *entry, int follow_links TSRMLS_DC) /* {{{ */
{
	if (follow_links && entry->link) {
		phar_entry_info *link_entry = phar_get_link_source(entry TSRMLS_CC);

		if (link_entry && link_entry != entry) {
			return phar_get_efp(link_entry, 1 TSRMLS_CC);
		}
	}

	if (phar_get_fp_type(entry TSRMLS_CC) == PHAR_FP) {
		if (!phar_get_entrypfp(entry TSRMLS_CC)) {
			/* re-open just in time for cases where our refcount reached 0 on the phar archive */
			phar_open_archive_fp(entry->phar TSRMLS_CC);
		}
		return phar_get_entrypfp(entry TSRMLS_CC);
	} else if (phar_get_fp_type(entry TSRMLS_CC) == PHAR_UFP) {
		return phar_get_entrypufp(entry TSRMLS_CC);
	} else if (entry->fp_type == PHAR_MOD) {
		return entry->fp;
	} else {
		/* temporary manifest entry */
		if (!entry->fp) {
			entry->fp = php_stream_open_wrapper(entry->tmp, "rb", STREAM_MUST_SEEK|0, NULL);
		}
		return entry->fp;
	}
}
/* }}} */

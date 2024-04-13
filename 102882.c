phar_entry_info *phar_get_link_source(phar_entry_info *entry TSRMLS_DC) /* {{{ */
{
	phar_entry_info *link_entry;
	char *link;

	if (!entry->link) {
		return entry;
	}

	link = phar_get_link_location(entry TSRMLS_CC);
	if (SUCCESS == zend_hash_find(&(entry->phar->manifest), entry->link, strlen(entry->link), (void **)&link_entry) ||
		SUCCESS == zend_hash_find(&(entry->phar->manifest), link, strlen(link), (void **)&link_entry)) {
		if (link != entry->link) {
			efree(link);
		}
		return phar_get_link_source(link_entry TSRMLS_CC);
	} else {
		if (link != entry->link) {
			efree(link);
		}
		return NULL;
	}
}
/* }}} */

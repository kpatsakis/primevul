static void user_config_cache_entry_dtor(user_config_cache_entry *entry)
{
	zend_hash_destroy(entry->user_config);
	free(entry->user_config);
}

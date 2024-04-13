static void php_cgi_ini_activate_user_config(char *path, int path_len, const char *doc_root, int doc_root_len, int start TSRMLS_DC)
{
	char *ptr;
	user_config_cache_entry *new_entry, *entry;
	time_t request_time = sapi_get_request_time(TSRMLS_C);

	/* Find cached config entry: If not found, create one */
	if (zend_hash_find(&CGIG(user_config_cache), path, path_len + 1, (void **) &entry) == FAILURE) {
		new_entry = pemalloc(sizeof(user_config_cache_entry), 1);
		new_entry->expires = 0;
		new_entry->user_config = (HashTable *) pemalloc(sizeof(HashTable), 1);
		zend_hash_init(new_entry->user_config, 0, NULL, (dtor_func_t) config_zval_dtor, 1);
		zend_hash_update(&CGIG(user_config_cache), path, path_len + 1, new_entry, sizeof(user_config_cache_entry), (void **) &entry);
		free(new_entry);
	}

	/* Check whether cache entry has expired and rescan if it is */
	if (request_time > entry->expires) {
		char *real_path = NULL;
		int real_path_len;
		char *s1, *s2;
		int s_len;

		/* Clear the expired config */
		zend_hash_clean(entry->user_config);

		if (!IS_ABSOLUTE_PATH(path, path_len)) {
			real_path = tsrm_realpath(path, NULL TSRMLS_CC);
			if (real_path == NULL) {
				return;
			}
			real_path_len = strlen(real_path);
			path = real_path;
			path_len = real_path_len;
		}

		if (path_len > doc_root_len) {
			s1 = (char *) doc_root;
			s2 = path;
			s_len = doc_root_len;
		} else {
			s1 = path;
			s2 = (char *) doc_root;
			s_len = path_len;
		}

		/* we have to test if path is part of DOCUMENT_ROOT.
		  if it is inside the docroot, we scan the tree up to the docroot 
			to find more user.ini, if not we only scan the current path.
		  */
#ifdef PHP_WIN32
		if (strnicmp(s1, s2, s_len) == 0) {
#else 
		if (strncmp(s1, s2, s_len) == 0) {
#endif
			ptr = s2 + start;  /* start is the point where doc_root ends! */
			while ((ptr = strchr(ptr, DEFAULT_SLASH)) != NULL) {
				*ptr = 0;
				php_parse_user_ini_file(path, PG(user_ini_filename), entry->user_config TSRMLS_CC);
				*ptr = '/';
				ptr++;
			}
		} else {
			php_parse_user_ini_file(path, PG(user_ini_filename), entry->user_config TSRMLS_CC);
		}

		if (real_path) {
			free(real_path);
		}
		entry->expires = request_time + PG(user_ini_cache_ttl);
	}

	/* Activate ini entries with values from the user config hash */
	php_ini_activate_config(entry->user_config, PHP_INI_PERDIR, PHP_INI_STAGE_HTACCESS TSRMLS_CC);
}
/* }}} */

static int sapi_cgi_activate(TSRMLS_D)
{
	char *path, *doc_root, *server_name;
	uint path_len, doc_root_len, server_name_len;

	/* PATH_TRANSLATED should be defined at this stage but better safe than sorry :) */
	if (!SG(request_info).path_translated) {

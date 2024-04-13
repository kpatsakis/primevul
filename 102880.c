phar_entry_info *phar_get_entry_info_dir(phar_archive_data *phar, char *path, int path_len, char dir, char **error, int security TSRMLS_DC) /* {{{ */
{
	const char *pcr_error;
	phar_entry_info *entry;
	int is_dir;

#ifdef PHP_WIN32
	phar_unixify_path_separators(path, path_len);
#endif

	is_dir = (path_len && (path[path_len - 1] == '/')) ? 1 : 0;

	if (error) {
		*error = NULL;
	}

	if (security && path_len >= sizeof(".phar")-1 && !memcmp(path, ".phar", sizeof(".phar")-1)) {
		if (error) {
			spprintf(error, 4096, "phar error: cannot directly access magic \".phar\" directory or files within it");
		}
		return NULL;
	}

	if (!path_len && !dir) {
		if (error) {
			spprintf(error, 4096, "phar error: invalid path \"%s\" must not be empty", path);
		}
		return NULL;
	}

	if (phar_path_check(&path, &path_len, &pcr_error) > pcr_is_ok) {
		if (error) {
			spprintf(error, 4096, "phar error: invalid path \"%s\" contains %s", path, pcr_error);
		}
		return NULL;
	}

	if (!phar->manifest.arBuckets) {
		return NULL;
	}

	if (is_dir) {
		if (!path_len || path_len == 1) {
			return NULL;
		}
		path_len--;
	}

	if (SUCCESS == zend_hash_find(&phar->manifest, path, path_len, (void**)&entry)) {
		if (entry->is_deleted) {
			/* entry is deleted, but has not been flushed to disk yet */
			return NULL;
		}
		if (entry->is_dir && !dir) {
			if (error) {
				spprintf(error, 4096, "phar error: path \"%s\" is a directory", path);
			}
			return NULL;
		}
		if (!entry->is_dir && dir == 2) {
			/* user requested a directory, we must return one */
			if (error) {
				spprintf(error, 4096, "phar error: path \"%s\" exists and is a not a directory", path);
			}
			return NULL;
		}
		return entry;
	}

	if (dir) {
		if (zend_hash_exists(&phar->virtual_dirs, path, path_len)) {
			/* a file or directory exists in a sub-directory of this path */
			entry = (phar_entry_info *) ecalloc(1, sizeof(phar_entry_info));
			/* this next line tells PharFileInfo->__destruct() to efree the filename */
			entry->is_temp_dir = entry->is_dir = 1;
			entry->filename = (char *) estrndup(path, path_len + 1);
			entry->filename_len = path_len;
			entry->phar = phar;
			return entry;
		}
	}

	if (phar->mounted_dirs.arBuckets && zend_hash_num_elements(&phar->mounted_dirs)) {
		phar_zstr key;
		char *str_key;
		ulong unused;
		uint keylen;

		zend_hash_internal_pointer_reset(&phar->mounted_dirs);
		while (FAILURE != zend_hash_has_more_elements(&phar->mounted_dirs)) {
			if (HASH_KEY_NON_EXISTENT == zend_hash_get_current_key_ex(&phar->mounted_dirs, &key, &keylen, &unused, 0, NULL)) {
				break;
			}

			PHAR_STR(key, str_key);

			if ((int)keylen >= path_len || strncmp(str_key, path, keylen)) {
				PHAR_STR_FREE(str_key);
				continue;
			} else {
				char *test;
				int test_len;
				php_stream_statbuf ssb;

				if (SUCCESS != zend_hash_find(&phar->manifest, str_key, keylen, (void **) &entry)) {
					if (error) {
						spprintf(error, 4096, "phar internal error: mounted path \"%s\" could not be retrieved from manifest", str_key);
					}
					PHAR_STR_FREE(str_key);
					return NULL;
				}

				if (!entry->tmp || !entry->is_mounted) {
					if (error) {
						spprintf(error, 4096, "phar internal error: mounted path \"%s\" is not properly initialized as a mounted path", str_key);
					}
					PHAR_STR_FREE(str_key);
					return NULL;
				}
				PHAR_STR_FREE(str_key);

				test_len = spprintf(&test, MAXPATHLEN, "%s%s", entry->tmp, path + keylen);

				if (SUCCESS != php_stream_stat_path(test, &ssb)) {
					efree(test);
					return NULL;
				}

				if (ssb.sb.st_mode & S_IFDIR && !dir) {
					efree(test);
					if (error) {
						spprintf(error, 4096, "phar error: path \"%s\" is a directory", path);
					}
					return NULL;
				}

				if ((ssb.sb.st_mode & S_IFDIR) == 0 && dir) {
					efree(test);
					/* user requested a directory, we must return one */
					if (error) {
						spprintf(error, 4096, "phar error: path \"%s\" exists and is a not a directory", path);
					}
					return NULL;
				}

				/* mount the file just in time */
				if (SUCCESS != phar_mount_entry(phar, test, test_len, path, path_len TSRMLS_CC)) {
					efree(test);
					if (error) {
						spprintf(error, 4096, "phar error: path \"%s\" exists as file \"%s\" and could not be mounted", path, test);
					}
					return NULL;
				}

				efree(test);

				if (SUCCESS != zend_hash_find(&phar->manifest, path, path_len, (void**)&entry)) {
					if (error) {
						spprintf(error, 4096, "phar error: path \"%s\" exists as file \"%s\" and could not be retrieved after being mounted", path, test);
					}
					return NULL;
				}
				return entry;
			}
		}
	}

	return NULL;
}
/* }}} */

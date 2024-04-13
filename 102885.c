int phar_mount_entry(phar_archive_data *phar, char *filename, int filename_len, char *path, int path_len TSRMLS_DC) /* {{{ */
{
	phar_entry_info entry = {0};
	php_stream_statbuf ssb;
	int is_phar;
	const char *err;

	if (phar_path_check(&path, &path_len, &err) > pcr_is_ok) {
		return FAILURE;
	}

	if (path_len >= sizeof(".phar")-1 && !memcmp(path, ".phar", sizeof(".phar")-1)) {
		/* no creating magic phar files by mounting them */
		return FAILURE;
	}

	is_phar = (filename_len > 7 && !memcmp(filename, "phar://", 7));

	entry.phar = phar;
	entry.filename = estrndup(path, path_len);
#ifdef PHP_WIN32
	phar_unixify_path_separators(entry.filename, path_len);
#endif
	entry.filename_len = path_len;
	if (is_phar) {
		entry.tmp = estrndup(filename, filename_len);
	} else {
		entry.tmp = expand_filepath(filename, NULL TSRMLS_CC);
		if (!entry.tmp) {
			entry.tmp = estrndup(filename, filename_len);
		}
	}
#if PHP_API_VERSION < 20100412
	if (PG(safe_mode) && !is_phar && (!php_checkuid(entry.tmp, NULL, CHECKUID_CHECK_FILE_AND_DIR))) {
		efree(entry.tmp);
		efree(entry.filename);
		return FAILURE;
	}
#endif
	filename = entry.tmp;

	/* only check openbasedir for files, not for phar streams */
	if (!is_phar && php_check_open_basedir(filename TSRMLS_CC)) {
		efree(entry.tmp);
		efree(entry.filename);
		return FAILURE;
	}

	entry.is_mounted = 1;
	entry.is_crc_checked = 1;
	entry.fp_type = PHAR_TMP;

	if (SUCCESS != php_stream_stat_path(filename, &ssb)) {
		efree(entry.tmp);
		efree(entry.filename);
		return FAILURE;
	}

	if (ssb.sb.st_mode & S_IFDIR) {
		entry.is_dir = 1;
		if (SUCCESS != zend_hash_add(&phar->mounted_dirs, entry.filename, path_len, (void *)&(entry.filename), sizeof(char *), NULL)) {
			/* directory already mounted */
			efree(entry.tmp);
			efree(entry.filename);
			return FAILURE;
		}
	} else {
		entry.is_dir = 0;
		entry.uncompressed_filesize = entry.compressed_filesize = ssb.sb.st_size;
	}

	entry.flags = ssb.sb.st_mode;

	if (SUCCESS == zend_hash_add(&phar->manifest, entry.filename, path_len, (void*)&entry, sizeof(phar_entry_info), NULL)) {
		return SUCCESS;
	}

	efree(entry.tmp);
	efree(entry.filename);
	return FAILURE;
}
/* }}} */

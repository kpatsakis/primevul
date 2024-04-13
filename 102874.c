char *phar_find_in_include_path(char *filename, int filename_len, phar_archive_data **pphar TSRMLS_DC) /* {{{ */
{
	char *path, *fname, *arch, *entry, *ret, *test;
	int arch_len, entry_len, fname_len, ret_len;
	phar_archive_data *phar;

	if (pphar) {
		*pphar = NULL;
	} else {
		pphar = &phar;
	}

	if (!zend_is_executing(TSRMLS_C) || !PHAR_G(cwd)) {
		return phar_save_resolve_path(filename, filename_len TSRMLS_CC);
	}

	fname = (char*)zend_get_executed_filename(TSRMLS_C);
	fname_len = strlen(fname);

	if (PHAR_G(last_phar) && !memcmp(fname, "phar://", 7) && fname_len - 7 >= PHAR_G(last_phar_name_len) && !memcmp(fname + 7, PHAR_G(last_phar_name), PHAR_G(last_phar_name_len))) {
		arch = estrndup(PHAR_G(last_phar_name), PHAR_G(last_phar_name_len));
		arch_len = PHAR_G(last_phar_name_len);
		phar = PHAR_G(last_phar);
		goto splitted;
	}

	if (fname_len < 7 || memcmp(fname, "phar://", 7) || SUCCESS != phar_split_fname(fname, strlen(fname), &arch, &arch_len, &entry, &entry_len, 1, 0 TSRMLS_CC)) {
		return phar_save_resolve_path(filename, filename_len TSRMLS_CC);
	}

	efree(entry);

	if (*filename == '.') {
		int try_len;

		if (FAILURE == phar_get_archive(&phar, arch, arch_len, NULL, 0, NULL TSRMLS_CC)) {
			efree(arch);
			return phar_save_resolve_path(filename, filename_len TSRMLS_CC);
		}
splitted:
		if (pphar) {
			*pphar = phar;
		}

		try_len = filename_len;
		test = phar_fix_filepath(estrndup(filename, filename_len), &try_len, 1 TSRMLS_CC);

		if (*test == '/') {
			if (zend_hash_exists(&(phar->manifest), test + 1, try_len - 1)) {
				spprintf(&ret, 0, "phar://%s%s", arch, test);
				efree(arch);
				efree(test);
				return ret;
			}
		} else {
			if (zend_hash_exists(&(phar->manifest), test, try_len)) {
				spprintf(&ret, 0, "phar://%s/%s", arch, test);
				efree(arch);
				efree(test);
				return ret;
			}
		}
		efree(test);
	}

	spprintf(&path, MAXPATHLEN, "phar://%s/%s%c%s", arch, PHAR_G(cwd), DEFAULT_DIR_SEPARATOR, PG(include_path));
	efree(arch);
	ret = php_resolve_path(filename, filename_len, path TSRMLS_CC);
	efree(path);

	if (ret && strlen(ret) > 8 && !strncmp(ret, "phar://", 7)) {
		ret_len = strlen(ret);
		/* found phar:// */

		if (SUCCESS != phar_split_fname(ret, ret_len, &arch, &arch_len, &entry, &entry_len, 1, 0 TSRMLS_CC)) {
			return ret;
		}

		zend_hash_find(&(PHAR_GLOBALS->phar_fname_map), arch, arch_len, (void **) &pphar);

		if (!pphar && PHAR_G(manifest_cached)) {
			zend_hash_find(&cached_phars, arch, arch_len, (void **) &pphar);
		}

		efree(arch);
		efree(entry);
	}

	return ret;
}
/* }}} */

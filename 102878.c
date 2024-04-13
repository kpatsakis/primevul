int phar_get_entry_data(phar_entry_data **ret, char *fname, int fname_len, char *path, int path_len, char *mode, char allow_dir, char **error, int security TSRMLS_DC) /* {{{ */
{
	phar_archive_data *phar;
	phar_entry_info *entry;
	int for_write  = mode[0] != 'r' || mode[1] == '+';
	int for_append = mode[0] == 'a';
	int for_create = mode[0] != 'r';
	int for_trunc  = mode[0] == 'w';

	if (!ret) {
		return FAILURE;
	}

	*ret = NULL;

	if (error) {
		*error = NULL;
	}

	if (FAILURE == phar_get_archive(&phar, fname, fname_len, NULL, 0, error TSRMLS_CC)) {
		return FAILURE;
	}

	if (for_write && PHAR_G(readonly) && !phar->is_data) {
		if (error) {
			spprintf(error, 4096, "phar error: file \"%s\" in phar \"%s\" cannot be opened for writing, disabled by ini setting", path, fname);
		}
		return FAILURE;
	}

	if (!path_len) {
		if (error) {
			spprintf(error, 4096, "phar error: file \"\" in phar \"%s\" cannot be empty", fname);
		}
		return FAILURE;
	}
really_get_entry:
	if (allow_dir) {
		if ((entry = phar_get_entry_info_dir(phar, path, path_len, allow_dir, for_create && !PHAR_G(readonly) && !phar->is_data ? NULL : error, security TSRMLS_CC)) == NULL) {
			if (for_create && (!PHAR_G(readonly) || phar->is_data)) {
				return SUCCESS;
			}
			return FAILURE;
		}
	} else {
		if ((entry = phar_get_entry_info(phar, path, path_len, for_create && !PHAR_G(readonly) && !phar->is_data ? NULL : error, security TSRMLS_CC)) == NULL) {
			if (for_create && (!PHAR_G(readonly) || phar->is_data)) {
				return SUCCESS;
			}
			return FAILURE;
		}
	}

	if (for_write && phar->is_persistent) {
		if (FAILURE == phar_copy_on_write(&phar TSRMLS_CC)) {
			if (error) {
				spprintf(error, 4096, "phar error: file \"%s\" in phar \"%s\" cannot be opened for writing, could not make cached phar writeable", path, fname);
			}
			return FAILURE;
		} else {
			goto really_get_entry;
		}
	}

	if (entry->is_modified && !for_write) {
		if (error) {
			spprintf(error, 4096, "phar error: file \"%s\" in phar \"%s\" cannot be opened for reading, writable file pointers are open", path, fname);
		}
		return FAILURE;
	}

	if (entry->fp_refcount && for_write) {
		if (error) {
			spprintf(error, 4096, "phar error: file \"%s\" in phar \"%s\" cannot be opened for writing, readable file pointers are open", path, fname);
		}
		return FAILURE;
	}

	if (entry->is_deleted) {
		if (!for_create) {
			return FAILURE;
		}
		entry->is_deleted = 0;
	}

	if (entry->is_dir) {
		*ret = (phar_entry_data *) emalloc(sizeof(phar_entry_data));
		(*ret)->position = 0;
		(*ret)->fp = NULL;
		(*ret)->phar = phar;
		(*ret)->for_write = for_write;
		(*ret)->internal_file = entry;
		(*ret)->is_zip = entry->is_zip;
		(*ret)->is_tar = entry->is_tar;

		if (!phar->is_persistent) {
			++(entry->phar->refcount);
			++(entry->fp_refcount);
		}

		return SUCCESS;
	}

	if (entry->fp_type == PHAR_MOD) {
		if (for_trunc) {
			if (FAILURE == phar_create_writeable_entry(phar, entry, error TSRMLS_CC)) {
				return FAILURE;
			}
		} else if (for_append) {
			phar_seek_efp(entry, 0, SEEK_END, 0, 0 TSRMLS_CC);
		}
	} else {
		if (for_write) {
			if (entry->link) {
				efree(entry->link);
				entry->link = NULL;
				entry->tar_type = (entry->is_tar ? TAR_FILE : '\0');
			}

			if (for_trunc) {
				if (FAILURE == phar_create_writeable_entry(phar, entry, error TSRMLS_CC)) {
					return FAILURE;
				}
			} else {
				if (FAILURE == phar_separate_entry_fp(entry, error TSRMLS_CC)) {
					return FAILURE;
				}
			}
		} else {
			if (FAILURE == phar_open_entry_fp(entry, error, 1 TSRMLS_CC)) {
				return FAILURE;
			}
		}
	}

	*ret = (phar_entry_data *) emalloc(sizeof(phar_entry_data));
	(*ret)->position = 0;
	(*ret)->phar = phar;
	(*ret)->for_write = for_write;
	(*ret)->internal_file = entry;
	(*ret)->is_zip = entry->is_zip;
	(*ret)->is_tar = entry->is_tar;
	(*ret)->fp = phar_get_efp(entry, 1 TSRMLS_CC);
	if (entry->link) {
		phar_entry_info *link = phar_get_link_source(entry TSRMLS_CC);
		if(!link) {
			efree(*ret);
			return FAILURE;
		}
		(*ret)->zero = phar_get_fp_offset(link TSRMLS_CC);
	} else {
		(*ret)->zero = phar_get_fp_offset(entry TSRMLS_CC);
	}

	if (!phar->is_persistent) {
		++(entry->fp_refcount);
		++(entry->phar->refcount);
	}

	return SUCCESS;
}
/* }}} */

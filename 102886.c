int phar_open_archive_fp(phar_archive_data *phar TSRMLS_DC) /* {{{ */
{
	if (phar_get_pharfp(phar TSRMLS_CC)) {
		return SUCCESS;
	}

	if (php_check_open_basedir(phar->fname TSRMLS_CC)) {
		return FAILURE;
	}

	phar_set_pharfp(phar, php_stream_open_wrapper(phar->fname, "rb", IGNORE_URL|STREAM_MUST_SEEK|0, NULL) TSRMLS_CC);

	if (!phar_get_pharfp(phar TSRMLS_CC)) {
		return FAILURE;
	}

	return SUCCESS;
}
/* }}} */

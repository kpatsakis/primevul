PHP_FUNCTION(is_uploaded_file)
{
	char *path;
	int path_len;

	if (!SG(rfc1867_uploaded_files)) {
		RETURN_FALSE;
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
		return;
	}

	if (zend_hash_exists(SG(rfc1867_uploaded_files), path, path_len + 1)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

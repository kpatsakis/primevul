php_http_params_token_t **php_http_params_separator_init(zval *zv TSRMLS_DC)
{
	zval **sep;
	HashPosition pos;
	php_http_params_token_t **ret, **tmp;

	if (!zv) {
		return NULL;
	}

	zv = php_http_ztyp(IS_ARRAY, zv);
	ret = ecalloc(zend_hash_num_elements(Z_ARRVAL_P(zv)) + 1, sizeof(*ret));

	tmp = ret;
	FOREACH_VAL(pos, zv, sep) {
		zval *zt = php_http_ztyp(IS_STRING, *sep);

		if (Z_STRLEN_P(zt)) {
			*tmp = emalloc(sizeof(**tmp));
			(*tmp)->str = estrndup(Z_STRVAL_P(zt), (*tmp)->len = Z_STRLEN_P(zt));
			++tmp;
		}
		zval_ptr_dtor(&zt);
	}
	zval_ptr_dtor(&zv);

	*tmp = NULL;
	return ret;
}

static int phar_call_openssl_signverify(int is_sign, php_stream *fp, off_t end, char *key, int key_len, char **signature, int *signature_len TSRMLS_DC) /* {{{ */
{
	zend_fcall_info fci;
	zend_fcall_info_cache fcc;
	zval *zdata, *zsig, *zkey, *retval_ptr, **zp[3], *openssl;

	MAKE_STD_ZVAL(zdata);
	MAKE_STD_ZVAL(openssl);
	ZVAL_STRINGL(openssl, is_sign ? "openssl_sign" : "openssl_verify", is_sign ? sizeof("openssl_sign")-1 : sizeof("openssl_verify")-1, 1);
	MAKE_STD_ZVAL(zsig);
	ZVAL_STRINGL(zsig, *signature, *signature_len, 1);
	MAKE_STD_ZVAL(zkey);
	ZVAL_STRINGL(zkey, key, key_len, 1);
	zp[0] = &zdata;
	zp[1] = &zsig;
	zp[2] = &zkey;

	php_stream_rewind(fp);
	Z_TYPE_P(zdata) = IS_STRING;
	Z_STRLEN_P(zdata) = end;

	if (end != (off_t) php_stream_copy_to_mem(fp, &(Z_STRVAL_P(zdata)), (size_t) end, 0)) {
		zval_dtor(zdata);
		zval_dtor(zsig);
		zval_dtor(zkey);
		zval_dtor(openssl);
		efree(openssl);
		efree(zdata);
		efree(zkey);
		efree(zsig);
		return FAILURE;
	}

	if (FAILURE == zend_fcall_info_init(openssl, 0, &fci, &fcc, NULL, NULL TSRMLS_CC)) {
		zval_dtor(zdata);
		zval_dtor(zsig);
		zval_dtor(zkey);
		zval_dtor(openssl);
		efree(openssl);
		efree(zdata);
		efree(zkey);
		efree(zsig);
		return FAILURE;
	}

	fci.param_count = 3;
	fci.params = zp;
	Z_ADDREF_P(zdata);
	if (is_sign) {
		Z_SET_ISREF_P(zsig);
	} else {
		Z_ADDREF_P(zsig);
	}
	Z_ADDREF_P(zkey);

	fci.retval_ptr_ptr = &retval_ptr;

	if (FAILURE == zend_call_function(&fci, &fcc TSRMLS_CC)) {
		zval_dtor(zdata);
		zval_dtor(zsig);
		zval_dtor(zkey);
		zval_dtor(openssl);
		efree(openssl);
		efree(zdata);
		efree(zkey);
		efree(zsig);
		return FAILURE;
	}

	zval_dtor(openssl);
	efree(openssl);
	Z_DELREF_P(zdata);

	if (is_sign) {
		Z_UNSET_ISREF_P(zsig);
	} else {
		Z_DELREF_P(zsig);
	}
	Z_DELREF_P(zkey);

	zval_dtor(zdata);
	efree(zdata);
	zval_dtor(zkey);
	efree(zkey);

	switch (Z_TYPE_P(retval_ptr)) {
		default:
		case IS_LONG:
			zval_dtor(zsig);
			efree(zsig);
			if (1 == Z_LVAL_P(retval_ptr)) {
				efree(retval_ptr);
				return SUCCESS;
			}
			efree(retval_ptr);
			return FAILURE;
		case IS_BOOL:
			efree(retval_ptr);
			if (Z_BVAL_P(retval_ptr)) {
				*signature = estrndup(Z_STRVAL_P(zsig), Z_STRLEN_P(zsig));
				*signature_len = Z_STRLEN_P(zsig);
				zval_dtor(zsig);
				efree(zsig);
				return SUCCESS;
			}
			zval_dtor(zsig);
			efree(zsig);
			return FAILURE;
	}
}
/* }}} */

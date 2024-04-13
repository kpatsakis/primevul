php_http_buffer_t *php_http_params_to_string(php_http_buffer_t *buf, HashTable *params, const char *pss, size_t psl, const char *ass, size_t asl, const char *vss, size_t vsl, unsigned flags TSRMLS_DC)
{
	zval **zparam;
	HashPosition pos, pos1;
	php_http_array_hashkey_t key = php_http_array_hashkey_init(0), key1 = php_http_array_hashkey_init(0);
	zend_bool rfc5987 = 0;

	if (!buf) {
		buf = php_http_buffer_init(NULL);
	}

	FOREACH_HASH_KEYVAL(pos, params, key, zparam) {
		zval **zvalue, **zargs;

		if (Z_TYPE_PP(zparam) != IS_ARRAY) {
			zvalue = zparam;
		} else {
			if (SUCCESS != zend_hash_find(Z_ARRVAL_PP(zparam), ZEND_STRS("value"), (void *) &zvalue)) {
				if (SUCCESS != zend_hash_find(Z_ARRVAL_PP(zparam), ZEND_STRS("*rfc5987*"), (void *) &zvalue)) {
					zvalue = zparam;
				} else {
					rfc5987 = 1;
				}
			}
		}

		php_http_array_hashkey_stringify(&key);
		shift_param(buf, key.str, key.len - 1, *zvalue, pss, psl, ass, asl, vss, vsl, flags, rfc5987 TSRMLS_CC);
		php_http_array_hashkey_stringfree(&key);

		if (Z_TYPE_PP(zparam) == IS_ARRAY && SUCCESS != zend_hash_find(Z_ARRVAL_PP(zparam), ZEND_STRS("arguments"), (void *) &zvalue)) {
			if (zvalue == zparam) {
				continue;
			}
			zvalue = zparam;
		}

		if (Z_TYPE_PP(zvalue) == IS_ARRAY) {
			FOREACH_KEYVAL(pos1, *zvalue, key1, zargs) {
				if (zvalue == zparam && key1.type == HASH_KEY_IS_STRING && !strcmp(key1.str, "value")) {
					continue;
				}

				php_http_array_hashkey_stringify(&key1);
				shift_arg(buf, key1.str, key1.len - 1, *zargs, ass, asl, vss, vsl, flags TSRMLS_CC);
				php_http_array_hashkey_stringfree(&key1);
			}
		}
	}

	php_http_buffer_shrink(buf);
	php_http_buffer_fix(buf);

	return buf;
}

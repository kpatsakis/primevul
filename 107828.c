static inline void shift_rfc5987(php_http_buffer_t *buf, zval *zvalue, const char *vss, size_t vsl, unsigned flags TSRMLS_DC)
{
	HashTable *ht = HASH_OF(zvalue);
	zval **zdata, *tmp;
	php_http_array_hashkey_t key = php_http_array_hashkey_init(0);

	if (SUCCESS == zend_hash_get_current_data(ht, (void *) &zdata)
	&&	HASH_KEY_NON_EXISTENT != (key.type = zend_hash_get_current_key_ex(ht, &key.str, &key.len, &key.num, key.dup, NULL))
	) {
		php_http_array_hashkey_stringify(&key);
		php_http_buffer_appendf(buf, "*%.*sutf-8'%.*s'",
				(int) (vsl > INT_MAX ? INT_MAX : vsl), vss,
				(int) (key.len > INT_MAX ? INT_MAX : key.len), key.str);
		php_http_array_hashkey_stringfree(&key);

		tmp = php_http_zsep(1, IS_STRING, *zdata);
		prepare_value(flags | PHP_HTTP_PARAMS_URLENCODED, tmp TSRMLS_CC);
		php_http_buffer_append(buf, Z_STRVAL_P(tmp), Z_STRLEN_P(tmp));
		zval_ptr_dtor(&tmp);
	}
}

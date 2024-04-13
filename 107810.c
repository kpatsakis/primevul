static void prepare_dimension(php_http_buffer_t *buf, php_http_buffer_t *keybuf, zval *zvalue, const char *pss, size_t psl, const char *vss, size_t vsl, unsigned flags TSRMLS_DC)
{
	HashTable *ht = HASH_OF(zvalue);
	HashPosition pos;
	php_http_array_hashkey_t key = php_http_array_hashkey_init(0);
	zval **val;
	php_http_buffer_t prefix;

	if (!ht->nApplyCount++) {
		php_http_buffer_init(&prefix);
		php_http_buffer_append(&prefix, keybuf->data, keybuf->used);

		FOREACH_HASH_KEYVAL(pos, ht, key, val) {
			if (key.type == HASH_KEY_IS_STRING && !*key.str) {
				/* only public properties */
				continue;
			}

			php_http_buffer_appends(&prefix, "[");
			if (key.type == HASH_KEY_IS_STRING) {
				php_http_buffer_append(&prefix, key.str, key.len - 1);
			} else {
				php_http_buffer_appendf(&prefix, "%lu", key.num);
			}
			php_http_buffer_appends(&prefix, "]");

			if (Z_TYPE_PP(val) == IS_ARRAY || Z_TYPE_PP(val) == IS_OBJECT) {
				prepare_dimension(buf, &prefix, *val, pss, psl, vss, vsl, flags TSRMLS_CC);
			} else {
				zval *cpy = php_http_ztyp(IS_STRING, *val);

				shift_key(buf, prefix.data, prefix.used, pss, psl, flags TSRMLS_CC);
				shift_val(buf, cpy, vss, vsl, flags TSRMLS_CC);
				zval_ptr_dtor(&cpy);
			}

			php_http_buffer_cut(&prefix, keybuf->used, prefix.used - keybuf->used);
		}
		php_http_buffer_dtor(&prefix);
	}
	--ht->nApplyCount;
}

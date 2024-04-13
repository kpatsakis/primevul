static void php_simple_ini_parser_cb(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *arr TSRMLS_DC)
{
	zval *element;

	switch (callback_type) {

		case ZEND_INI_PARSER_ENTRY:
			if (!arg2) {
				/* bare string - nothing to do */
				break;
			}
			ALLOC_ZVAL(element);
			MAKE_COPY_ZVAL(&arg2, element);
			zend_symtable_update(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, &element, sizeof(zval *), NULL);
			break;

		case ZEND_INI_PARSER_POP_ENTRY:
		{
			zval *hash, **find_hash;

			if (!arg2) {
				/* bare string - nothing to do */
				break;
			}

			if (!(Z_STRLEN_P(arg1) > 1 && Z_STRVAL_P(arg1)[0] == '0') && is_numeric_string(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1), NULL, NULL, 0) == IS_LONG) {
				ulong key = (ulong) zend_atol(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1));
				if (zend_hash_index_find(Z_ARRVAL_P(arr), key, (void **) &find_hash) == FAILURE) {
					ALLOC_ZVAL(hash);
					INIT_PZVAL(hash);
					array_init(hash);

					zend_hash_index_update(Z_ARRVAL_P(arr), key, &hash, sizeof(zval *), NULL);
				} else {
					hash = *find_hash;
				}
			} else {
				if (zend_hash_find(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, (void **) &find_hash) == FAILURE) {
					ALLOC_ZVAL(hash);
					INIT_PZVAL(hash);
					array_init(hash);

					zend_hash_update(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, &hash, sizeof(zval *), NULL);
				} else {
					hash = *find_hash;
				}
			}

			if (Z_TYPE_P(hash) != IS_ARRAY) {
				zval_dtor(hash);
				INIT_PZVAL(hash);
				array_init(hash);
			}

			ALLOC_ZVAL(element);
			MAKE_COPY_ZVAL(&arg2, element);

			if (arg3 && Z_STRLEN_P(arg3) > 0) {
				add_assoc_zval_ex(hash, Z_STRVAL_P(arg3), Z_STRLEN_P(arg3) + 1, element);
			} else {
				add_next_index_zval(hash, element);
			}
		}
		break;

		case ZEND_INI_PARSER_SECTION:
			break;
	}
}

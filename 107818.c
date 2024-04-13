static void sanitize_dimension(zval *zv TSRMLS_DC)
{
	zval *arr = NULL, *tmp = NULL, **cur = NULL;
	char *var = NULL, *ptr = Z_STRVAL_P(zv), *end = Z_STRVAL_P(zv) + Z_STRLEN_P(zv);
	long level = 0;

	MAKE_STD_ZVAL(arr);
	array_init(arr);
	cur = &arr;

	while (ptr < end) {
		if (!var) {
			var = ptr;
		}

		switch (*ptr) {
			case '[':
				if (++level > PG(max_input_nesting_level)) {
					zval_ptr_dtor(&arr);
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Max input nesting level of %ld exceeded", (long) PG(max_input_nesting_level));
					return;
				}
				if (ptr - var == 0) {
					++var;
					break;
				}
				/* no break */

			case ']':

				MAKE_STD_ZVAL(tmp);
				ZVAL_NULL(tmp);
				convert_to_array(*cur);

				if (ptr - var) {
					char chr = *ptr;
					*ptr = '\0';
					zend_symtable_update(Z_ARRVAL_PP(cur), var, ptr - var + 1, (void *) &tmp, sizeof(zval *), (void *) &cur);
					*ptr = chr;
				} else {
					zend_hash_next_index_insert(Z_ARRVAL_PP(cur), (void *) &tmp, sizeof(zval *), (void *) &cur);
				}

				var = NULL;
				break;
		}

		++ptr;
	}

	if (zend_hash_num_elements(Z_ARRVAL_P(arr))) {
		zval_dtor(zv);
#if PHP_VERSION_ID >= 50400
		ZVAL_COPY_VALUE(zv, arr);
#else
		zv->value = arr->value;
		Z_TYPE_P(zv) = Z_TYPE_P(arr);
#endif
		FREE_ZVAL(arr);
	} else {
		zval_ptr_dtor(&arr);
	}
}

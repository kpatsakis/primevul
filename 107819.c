static inline void sanitize_escaped(zval *zv TSRMLS_DC)
{
	if (Z_STRVAL_P(zv)[0] == '"' && Z_STRVAL_P(zv)[Z_STRLEN_P(zv) - 1] == '"') {
		size_t deq_len = Z_STRLEN_P(zv) - 2;
		char *deq = estrndup(Z_STRVAL_P(zv) + 1, deq_len);

		zval_dtor(zv);
		ZVAL_STRINGL(zv, deq, deq_len, 0);
	}

	php_stripcslashes(Z_STRVAL_P(zv), &Z_STRLEN_P(zv));
}

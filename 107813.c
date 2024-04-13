static inline void prepare_rfc5988(zval *zv TSRMLS_DC)
{
	if (Z_TYPE_P(zv) != IS_STRING) {
		zval_dtor(zv);
		ZVAL_EMPTY_STRING(zv);
	}
}

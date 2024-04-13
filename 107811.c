static inline void prepare_escaped(zval *zv TSRMLS_DC)
{
	if (Z_TYPE_P(zv) == IS_STRING) {
		quote_string(zv, 0 TSRMLS_CC);
	} else {
		zval_dtor(zv);
		ZVAL_EMPTY_STRING(zv);
	}
}

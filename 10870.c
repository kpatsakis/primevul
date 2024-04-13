d_lite_minus(VALUE self, VALUE other)
{
    if (k_date_p(other))
	return minus_dd(self, other);

    switch (TYPE(other)) {
      case T_FIXNUM:
	return d_lite_plus(self, LONG2NUM(-FIX2LONG(other)));
      case T_FLOAT:
	return d_lite_plus(self, DBL2NUM(-RFLOAT_VALUE(other)));
      default:
	expect_numeric(other);
	/* fall through */
      case T_BIGNUM:
      case T_RATIONAL:
	return d_lite_plus(self, f_negate(other));
    }
}
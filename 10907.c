wholenum_p(VALUE x)
{
    if (FIXNUM_P(x))
	return 1;
    switch (TYPE(x)) {
      case T_BIGNUM:
	return 1;
      case T_FLOAT:
	{
	    double d = RFLOAT_VALUE(x);
	    return round(d) == d;
	}
	break;
      case T_RATIONAL:
	{
	    VALUE den = rb_rational_den(x);
	    return FIXNUM_P(den) && FIX2LONG(den) == 1;
	}
	break;
    }
    return 0;
}
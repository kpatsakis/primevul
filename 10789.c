canon(VALUE x)
{
    if (RB_TYPE_P(x, T_RATIONAL)) {
	VALUE den = rb_rational_den(x);
	if (FIXNUM_P(den) && FIX2LONG(den) == 1)
	    return rb_rational_num(x);
    }
    return x;
}
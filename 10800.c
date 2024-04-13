ms_to_sec(VALUE m)
{
    if (FIXNUM_P(m))
	return rb_rational_new2(m, INT2FIX(SECOND_IN_MILLISECONDS));
    return f_quo(m, INT2FIX(SECOND_IN_MILLISECONDS));
}
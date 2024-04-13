ns_to_day(VALUE n)
{
    if (FIXNUM_P(n))
	return rb_rational_new2(n, day_in_nanoseconds);
    return f_quo(n, day_in_nanoseconds);
}
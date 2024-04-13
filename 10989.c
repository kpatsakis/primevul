sec_to_ns(VALUE s)
{
    if (safe_mul_p(s, SECOND_IN_NANOSECONDS))
	return LONG2FIX(FIX2LONG(s) * SECOND_IN_NANOSECONDS);
    return f_mul(s, INT2FIX(SECOND_IN_NANOSECONDS));
}
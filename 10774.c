sec_to_ms(VALUE s)
{
    if (safe_mul_p(s, SECOND_IN_MILLISECONDS))
	return LONG2FIX(FIX2LONG(s) * SECOND_IN_MILLISECONDS);
    return f_mul(s, INT2FIX(SECOND_IN_MILLISECONDS));
}
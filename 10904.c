day_to_sec(VALUE d)
{
    if (safe_mul_p(d, DAY_IN_SECONDS))
	return LONG2FIX(FIX2LONG(d) * DAY_IN_SECONDS);
    return f_mul(d, INT2FIX(DAY_IN_SECONDS));
}
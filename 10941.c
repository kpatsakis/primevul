encode_jd(VALUE nth, int jd, VALUE *rjd)
{
    if (f_zero_p(nth)) {
	*rjd = INT2FIX(jd);
	return;
    }
    *rjd = f_add(f_mul(INT2FIX(CM_PERIOD), nth), INT2FIX(jd));
}
decode_jd(VALUE jd, VALUE *nth, int *rjd)
{
    *nth = f_idiv(jd, INT2FIX(CM_PERIOD));
    if (f_zero_p(*nth)) {
	*rjd = FIX2INT(jd);
	return;
    }
    *rjd = FIX2INT(f_mod(jd, INT2FIX(CM_PERIOD)));
}
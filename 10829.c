decode_year(VALUE y, double style,
	    VALUE *nth, int *ry)
{
    int period;
    VALUE t;

    period = (style < 0) ?
	CM_PERIOD_GCY :
	CM_PERIOD_JCY;
    if (FIXNUM_P(y)) {
	long iy, it, inth;

	iy = FIX2LONG(y);
	if (iy >= (FIXNUM_MAX - 4712))
	    goto big;
	it = iy + 4712; /* shift */
	inth = DIV(it, ((long)period));
	*nth = LONG2FIX(inth);
	if (inth)
	    it = MOD(it, ((long)period));
	*ry = (int)it - 4712; /* unshift */
	return;
    }
  big:
    t = f_add(y, INT2FIX(4712)); /* shift */
    *nth = f_idiv(t, INT2FIX(period));
    if (f_nonzero_p(*nth))
	t = f_mod(t, INT2FIX(period));
    *ry = FIX2INT(t) - 4712; /* unshift */
}
d_trunc(VALUE d, VALUE *fr)
{
    VALUE rd;

    if (wholenum_p(d)) {
	rd = to_integer(d);
	*fr = INT2FIX(0);
    }
    else {
	rd = f_idiv(d, INT2FIX(1));
	*fr = f_mod(d, INT2FIX(1));
    }
    return rd;
}
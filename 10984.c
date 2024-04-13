h_trunc(VALUE h, VALUE *fr)
{
    VALUE rh;

    if (wholenum_p(h)) {
	rh = to_integer(h);
	*fr = INT2FIX(0);
    }
    else {
	rh = f_idiv(h, INT2FIX(1));
	*fr = f_mod(h, INT2FIX(1));
	*fr = f_quo(*fr, INT2FIX(24));
    }
    return rh;
}
s_trunc(VALUE s, VALUE *fr)
{
    VALUE rs;

    if (wholenum_p(s)) {
	rs = to_integer(s);
	*fr = INT2FIX(0);
    }
    else {
	rs = f_idiv(s, INT2FIX(1));
	*fr = f_mod(s, INT2FIX(1));
	*fr = f_quo(*fr, INT2FIX(86400));
    }
    return rs;
}
div_df(VALUE d, VALUE *f)
{
    VALUE s = day_to_sec(d);

    if (f)
	*f = f_mod(s, INT2FIX(1));
    return f_floor(s);
}
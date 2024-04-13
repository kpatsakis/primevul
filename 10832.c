m_amjd(union DateData *x)
{
    VALUE r, sf;
    int df;

    r = m_real_jd(x);
    if (FIXNUM_P(r) && FIX2LONG(r) >= (FIXNUM_MIN + 2400001)) {
	long ir = FIX2LONG(r);
	ir -= 2400001;
	r = rb_rational_new1(LONG2FIX(ir));
    }
    else
	r = rb_rational_new1(f_sub(m_real_jd(x),
				   INT2FIX(2400001)));

    if (simple_dat_p(x))
	return r;

    df = m_df(x);
    if (df)
	r = f_add(r, isec_to_day(df));
    sf = m_sf(x);
    if (f_nonzero_p(sf))
	r = f_add(r, ns_to_day(sf));

    return r;
}
m_ajd(union DateData *x)
{
    VALUE r, sf;
    int df;

    if (simple_dat_p(x)) {
	r = m_real_jd(x);
	if (FIXNUM_P(r) && FIX2LONG(r) <= (FIXNUM_MAX / 2)) {
	    long ir = FIX2LONG(r);
	    ir = ir * 2 - 1;
	    return rb_rational_new2(LONG2FIX(ir), INT2FIX(2));
	}
	else
	    return rb_rational_new2(f_sub(f_mul(r,
						INT2FIX(2)),
					  INT2FIX(1)),
				    INT2FIX(2));
    }

    r = m_real_jd(x);
    df = m_df(x);
    df -= HALF_DAYS_IN_SECONDS;
    if (df)
	r = f_add(r, isec_to_day(df));
    sf = m_sf(x);
    if (f_nonzero_p(sf))
	r = f_add(r, ns_to_day(sf));

    return r;
}
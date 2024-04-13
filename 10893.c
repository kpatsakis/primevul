get_c_civil(union DateData *x)
{
    assert(complex_dat_p(x));
    if (!have_civil_p(x)) {
#ifndef USE_PACK
	int jd, y, m, d;
#else
	int jd, y, m, d, h, min, s;
#endif

	assert(have_jd_p(x));
	get_c_df(x);
	jd = jd_utc_to_local(x->c.jd, x->c.df, x->c.of);
	c_jd_to_civil(jd, c_virtual_sg(x), &y, &m, &d);
	x->c.year = y;
#ifndef USE_PACK
	x->c.mon = m;
	x->c.mday = d;
#else
	h = EX_HOUR(x->c.pc);
	min = EX_MIN(x->c.pc);
	s = EX_SEC(x->c.pc);
	x->c.pc = PACK5(m, d, h, min, s);
#endif
	x->c.flags |= HAVE_CIVIL;
    }
}
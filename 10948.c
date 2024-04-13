get_c_jd(union DateData *x)
{
    assert(complex_dat_p(x));
    if (!have_jd_p(x)) {
	int jd, ns;

	assert(have_civil_p(x));
#ifndef USE_PACK
	c_civil_to_jd(x->c.year, x->c.mon, x->c.mday,
		      c_virtual_sg(x), &jd, &ns);
#else
	c_civil_to_jd(x->c.year, EX_MON(x->c.pc), EX_MDAY(x->c.pc),
		      c_virtual_sg(x), &jd, &ns);
#endif

	get_c_time(x);
#ifndef USE_PACK
	x->c.jd = jd_local_to_utc(jd,
				  time_to_df(x->c.hour, x->c.min, x->c.sec),
				  x->c.of);
#else
	x->c.jd = jd_local_to_utc(jd,
				  time_to_df(EX_HOUR(x->c.pc),
					     EX_MIN(x->c.pc),
					     EX_SEC(x->c.pc)),
				  x->c.of);
#endif
	x->c.flags |= HAVE_JD;
    }
}
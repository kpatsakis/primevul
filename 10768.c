get_c_time(union DateData *x)
{
    assert(complex_dat_p(x));
    if (!have_time_p(x)) {
#ifndef USE_PACK
	int r;
	assert(have_df_p(x));
	r = df_utc_to_local(x->c.df, x->c.of);
	df_to_time(r, &x->c.hour, &x->c.min, &x->c.sec);
	x->c.flags |= HAVE_TIME;
#else
	int r, m, d, h, min, s;

	assert(have_df_p(x));
	m = EX_MON(x->c.pc);
	d = EX_MDAY(x->c.pc);
	r = df_utc_to_local(x->c.df, x->c.of);
	df_to_time(r, &h, &min, &s);
	x->c.pc = PACK5(m, d, h, min, s);
	x->c.flags |= HAVE_TIME;
#endif
    }
}
get_c_df(union DateData *x)
{
    assert(complex_dat_p(x));
    if (!have_df_p(x)) {
	assert(have_time_p(x));
#ifndef USE_PACK
	x->c.df = df_local_to_utc(time_to_df(x->c.hour, x->c.min, x->c.sec),
				  x->c.of);
#else
	x->c.df = df_local_to_utc(time_to_df(EX_HOUR(x->c.pc),
					     EX_MIN(x->c.pc),
					     EX_SEC(x->c.pc)),
				  x->c.of);
#endif
	x->c.flags |= HAVE_DF;
    }
}
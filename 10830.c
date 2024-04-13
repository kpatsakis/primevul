get_s_jd(union DateData *x)
{
    assert(simple_dat_p(x));
    if (!have_jd_p(x)) {
	int jd, ns;

	assert(have_civil_p(x));
#ifndef USE_PACK
	c_civil_to_jd(x->s.year, x->s.mon, x->s.mday,
		      s_virtual_sg(x), &jd, &ns);
#else
	c_civil_to_jd(x->s.year, EX_MON(x->s.pc), EX_MDAY(x->s.pc),
		      s_virtual_sg(x), &jd, &ns);
#endif
	x->s.jd = jd;
	x->s.flags |= HAVE_JD;
    }
}
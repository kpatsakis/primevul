clear_civil(union DateData *x)
{
    if (simple_dat_p(x)) {
	x->s.year = 0;
#ifndef USE_PACK
	x->s.mon = 0;
	x->s.mday = 0;
#else
	x->s.pc = 0;
#endif
	x->s.flags &= ~HAVE_CIVIL;
    }
    else {
	x->c.year = 0;
#ifndef USE_PACK
	x->c.mon = 0;
	x->c.mday = 0;
	x->c.hour = 0;
	x->c.min = 0;
	x->c.sec = 0;
#else
	x->c.pc = 0;
#endif
	x->c.flags &= ~(HAVE_CIVIL | HAVE_TIME);
    }
}
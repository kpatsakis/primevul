m_mon(union DateData *x)
{
    if (simple_dat_p(x)) {
	get_s_civil(x);
#ifndef USE_PACK
	return x->s.mon;
#else
	return EX_MON(x->s.pc);
#endif
    }
    else {
	get_c_civil(x);
#ifndef USE_PACK
	return x->c.mon;
#else
	return EX_MON(x->c.pc);
#endif
    }
}
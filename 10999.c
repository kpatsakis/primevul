m_mday(union DateData *x)
{
    if (simple_dat_p(x)) {
	get_s_civil(x);
#ifndef USE_PACK
	return x->s.mday;
#else
	return EX_MDAY(x->s.pc);
#endif
    }
    else {
	get_c_civil(x);
#ifndef USE_PACK
	return x->c.mday;
#else
	return EX_MDAY(x->c.pc);
#endif
    }
}
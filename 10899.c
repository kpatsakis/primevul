m_hour(union DateData *x)
{
    if (simple_dat_p(x))
	return 0;
    else {
	get_c_time(x);
#ifndef USE_PACK
	return x->c.hour;
#else
	return EX_HOUR(x->c.pc);
#endif
    }
}
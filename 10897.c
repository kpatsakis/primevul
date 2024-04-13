m_sec(union DateData *x)
{
    if (simple_dat_p(x))
	return 0;
    else {
	get_c_time(x);
#ifndef USE_PACK
	return x->c.sec;
#else
	return EX_SEC(x->c.pc);
#endif
    }
}
m_of(union DateData *x)
{
    if (simple_dat_p(x))
	return 0;
    else {
	get_c_jd(x);
	return x->c.of;
    }
}
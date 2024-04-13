m_jd(union DateData *x)
{
    if (simple_dat_p(x)) {
	get_s_jd(x);
	return x->s.jd;
    }
    else {
	get_c_jd(x);
	return x->c.jd;
    }
}
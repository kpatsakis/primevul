m_local_jd(union DateData *x)
{
    if (simple_dat_p(x)) {
	get_s_jd(x);
	return x->s.jd;
    }
    else {
	get_c_jd(x);
	get_c_df(x);
	return local_jd(x);
    }
}
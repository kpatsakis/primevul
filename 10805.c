m_sg(union DateData *x)
{
    if (simple_dat_p(x))
	return x->s.sg;
    else {
	get_c_jd(x);
	return x->c.sg;
    }
}
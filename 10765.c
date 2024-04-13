m_julian_p(union DateData *x)
{
    int jd;
    double sg;

    if (simple_dat_p(x)) {
	get_s_jd(x);
	jd = x->s.jd;
	sg = s_virtual_sg(x);
    }
    else {
	get_c_jd(x);
	jd = x->c.jd;
	sg = c_virtual_sg(x);
    }
    if (isinf(sg))
	return sg == positive_inf;
    return jd < sg;
}
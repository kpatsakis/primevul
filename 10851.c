m_year(union DateData *x)
{
    if (simple_dat_p(x)) {
	get_s_civil(x);
	return x->s.year;
    }
    else {
	get_c_civil(x);
	return x->c.year;
    }
}
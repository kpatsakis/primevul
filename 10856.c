d_lite_day_fraction(VALUE self)
{
    get_d1(self);
    if (simple_dat_p(dat))
	return INT2FIX(0);
    return m_fr(dat);
}
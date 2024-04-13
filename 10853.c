d_lite_leap_p(VALUE self)
{
    int rjd, ns, ry, rm, rd;

    get_d1(self);
    if (m_gregorian_p(dat))
	return f_boolcast(c_gregorian_leap_p(m_year(dat)));

    c_civil_to_jd(m_year(dat), 3, 1, m_virtual_sg(dat),
		  &rjd, &ns);
    c_jd_to_civil(rjd - 1, m_virtual_sg(dat), &ry, &rm, &rd);
    return f_boolcast(rd == 29);
}
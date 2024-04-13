m_real_year(union DateData *x)
{
    VALUE nth, ry;
    int year;

    nth = m_nth(x);
    year = m_year(x);

    if (f_zero_p(nth))
	return INT2FIX(year);

    encode_year(nth, year,
		m_gregorian_p(x) ? -1 : +1,
		&ry);
    return ry;
}
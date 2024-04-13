d_lite_equal(VALUE self, VALUE other)
{
    if (!k_date_p(other))
	return equal_gen(self, other);

    {
	get_d2(self, other);

	if (!(m_gregorian_p(adat) == m_gregorian_p(bdat)))
	    return equal_gen(self, other);

	{
	    VALUE a_nth, b_nth;
	    int a_jd, b_jd;

	    m_canonicalize_jd(self, adat);
	    m_canonicalize_jd(other, bdat);
	    a_nth = m_nth(adat);
	    b_nth = m_nth(bdat);
	    a_jd = m_local_jd(adat);
	    b_jd = m_local_jd(bdat);
	    if (f_eqeq_p(a_nth, b_nth) &&
		a_jd == b_jd)
		return Qtrue;
	    return Qfalse;
	}
    }
}
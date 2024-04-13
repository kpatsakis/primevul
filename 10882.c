d_lite_cmp(VALUE self, VALUE other)
{
    if (!k_date_p(other))
	return cmp_gen(self, other);

    {
	get_d2(self, other);

	if (!(simple_dat_p(adat) && simple_dat_p(bdat) &&
	      m_gregorian_p(adat) == m_gregorian_p(bdat)))
	    return cmp_dd(self, other);

	{
	    VALUE a_nth, b_nth;
	    int a_jd, b_jd;

	    m_canonicalize_jd(self, adat);
	    m_canonicalize_jd(other, bdat);
	    a_nth = m_nth(adat);
	    b_nth = m_nth(bdat);
	    if (f_eqeq_p(a_nth, b_nth)) {
		a_jd = m_jd(adat);
		b_jd = m_jd(bdat);
		if (a_jd == b_jd) {
		    return INT2FIX(0);
		}
		else if (a_jd < b_jd) {
		    return INT2FIX(-1);
		}
		else {
		    return INT2FIX(1);
		}
	    }
	    else if (f_lt_p(a_nth, b_nth)) {
		return INT2FIX(-1);
	    }
	    else {
		return INT2FIX(1);
	    }
	}
    }
}
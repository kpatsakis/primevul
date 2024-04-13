cmp_dd(VALUE self, VALUE other)
{
    get_d2(self, other);

    {
	VALUE a_nth, b_nth,
	    a_sf, b_sf;
	int a_jd, b_jd,
	    a_df, b_df;

	m_canonicalize_jd(self, adat);
	m_canonicalize_jd(other, bdat);
	a_nth = m_nth(adat);
	b_nth = m_nth(bdat);
	if (f_eqeq_p(a_nth, b_nth)) {
	    a_jd = m_jd(adat);
	    b_jd = m_jd(bdat);
	    if (a_jd == b_jd) {
		a_df = m_df(adat);
		b_df = m_df(bdat);
		if (a_df == b_df) {
		    a_sf = m_sf(adat);
		    b_sf = m_sf(bdat);
		    if (f_eqeq_p(a_sf, b_sf)) {
			return INT2FIX(0);
		    }
		    else if (f_lt_p(a_sf, b_sf)) {
			return INT2FIX(-1);
		    }
		    else {
			return INT2FIX(1);
		    }
		}
		else if (a_df < b_df) {
		    return INT2FIX(-1);
		}
		else {
		    return INT2FIX(1);
		}
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
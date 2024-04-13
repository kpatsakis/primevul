date_to_time(VALUE self)
{
    get_d1a(self);

    if (m_julian_p(adat)) {
        VALUE tmp = d_lite_gregorian(self);
        get_d1b(tmp);
        adat = bdat;
    }

    return f_local3(rb_cTime,
        m_real_year(adat),
        INT2FIX(m_mon(adat)),
        INT2FIX(m_mday(adat)));
}
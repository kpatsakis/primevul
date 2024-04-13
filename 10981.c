datetime_to_date(VALUE self)
{
    get_d1a(self);

    if (simple_dat_p(adat)) {
	VALUE new = d_lite_s_alloc_simple(cDate);
	{
	    get_d1b(new);
	    bdat->s = adat->s;
	    bdat->s.jd = m_local_jd(adat);
	    return new;
	}
    }
    else {
	VALUE new = d_lite_s_alloc_simple(cDate);
	{
	    get_d1b(new);
	    copy_complex_to_simple(new, &bdat->s, &adat->c);
	    bdat->s.jd = m_local_jd(adat);
	    bdat->s.flags &= ~(HAVE_DF | HAVE_TIME | COMPLEX_DAT);
	    return new;
	}
    }
}
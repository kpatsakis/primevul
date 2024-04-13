m_fr(union DateData *x)
{
    if (simple_dat_p(x))
	return INT2FIX(0);
    else {
	int df;
	VALUE sf, fr;

	df = m_local_df(x);
	sf = m_sf(x);
	fr = isec_to_day(df);
	if (f_nonzero_p(sf))
	    fr = f_add(fr, ns_to_day(sf));
	return fr;
    }
}
tmx_m_secs(union DateData *x)
{
    VALUE s;
    int df;

    s = day_to_sec(f_sub(m_real_jd(x),
			 UNIX_EPOCH_IN_CJD));
    if (simple_dat_p(x))
	return s;
    df = m_df(x);
    if (df)
	s = f_add(s, INT2FIX(df));
    return s;
}
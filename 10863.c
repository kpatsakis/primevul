m_df(union DateData *x)
{
    if (simple_dat_p(x))
	return 0;
    else {
	get_c_df(x);
	return x->c.df;
    }
}
local_jd(union DateData *x)
{
    assert(complex_dat_p(x));
    assert(have_jd_p(x));
    assert(have_df_p(x));
    return jd_utc_to_local(x->c.jd, x->c.df, x->c.of);
}
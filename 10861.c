set_of(union DateData *x, int of)
{
    assert(complex_dat_p(x));
    get_c_jd(x);
    get_c_df(x);
    clear_civil(x);
    x->c.of = of;
}
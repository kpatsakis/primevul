decode_day(VALUE d, VALUE *jd, VALUE *df, VALUE *sf)
{
    VALUE f;

    *jd = div_day(d, &f);
    *df = div_df(f, &f);
    *sf = sec_to_ns(f);
}
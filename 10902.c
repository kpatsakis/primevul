date_s_gregorian_leap_p(VALUE klass, VALUE y)
{
    VALUE nth;
    int ry;

    check_numeric(y, "year");
    decode_year(y, -1, &nth, &ry);
    return f_boolcast(c_gregorian_leap_p(ry));
}
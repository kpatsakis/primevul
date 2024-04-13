time_to_date(VALUE self)
{
    VALUE y, nth, ret;
    int ry, m, d;

    y = f_year(self);
    m = FIX2INT(f_mon(self));
    d = FIX2INT(f_mday(self));

    decode_year(y, -1, &nth, &ry);

    ret = d_simple_new_internal(cDate,
				nth, 0,
				GREGORIAN,
				ry, m, d,
				HAVE_CIVIL);
    {
	get_d1(ret);
	set_sg(dat, DEFAULT_SG);
    }
    return ret;
}
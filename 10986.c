d_lite_monday_p(VALUE self)
{
    get_d1(self);
    return f_boolcast(m_wday(dat) == 1);
}
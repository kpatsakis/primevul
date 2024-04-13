d_lite_saturday_p(VALUE self)
{
    get_d1(self);
    return f_boolcast(m_wday(dat) == 6);
}
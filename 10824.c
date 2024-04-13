d_lite_wday(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_wday(dat));
}
d_lite_hour(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_hour(dat));
}
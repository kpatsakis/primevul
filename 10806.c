d_lite_mday(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_mday(dat));
}
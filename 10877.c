d_lite_min(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_min(dat));
}
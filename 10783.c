d_lite_sec(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_sec(dat));
}
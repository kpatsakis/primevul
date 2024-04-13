d_lite_wnum1(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_wnum1(dat));
}
d_lite_cwday(VALUE self)
{
    get_d1(self);
    return INT2FIX(m_cwday(dat));
}
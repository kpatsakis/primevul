d_lite_start(VALUE self)
{
    get_d1(self);
    return DBL2NUM(m_sg(dat));
}
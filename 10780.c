d_lite_gregorian_p(VALUE self)
{
    get_d1(self);
    return f_boolcast(m_gregorian_p(dat));
}
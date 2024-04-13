d_lite_marshal_dump_old(VALUE self)
{
    VALUE a;

    get_d1(self);

    a = rb_ary_new3(3,
		    m_ajd(dat),
		    m_of_in_day(dat),
		    DBL2NUM(m_sg(dat)));

    if (FL_TEST(self, FL_EXIVAR)) {
	rb_copy_generic_ivar(a, self);
	FL_SET(a, FL_EXIVAR);
    }

    return a;
}
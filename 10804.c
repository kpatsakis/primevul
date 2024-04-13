d_lite_marshal_dump(VALUE self)
{
    VALUE a;

    get_d1(self);

    a = rb_ary_new3(6,
		    m_nth(dat),
		    INT2FIX(m_jd(dat)),
		    INT2FIX(m_df(dat)),
		    m_sf(dat),
		    INT2FIX(m_of(dat)),
		    DBL2NUM(m_sg(dat)));

    if (FL_TEST(self, FL_EXIVAR)) {
	rb_copy_generic_ivar(a, self);
	FL_SET(a, FL_EXIVAR);
    }

    return a;
}
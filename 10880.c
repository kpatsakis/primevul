datetime_to_time(VALUE self)
{
    volatile VALUE dup = dup_obj(self);
    {
	VALUE t;

	get_d1(dup);

	t = rb_funcall(rb_cTime,
		   rb_intern("new"),
                   7,
		   m_real_year(dat),
		   INT2FIX(m_mon(dat)),
		   INT2FIX(m_mday(dat)),
		   INT2FIX(m_hour(dat)),
		   INT2FIX(m_min(dat)),
		   f_add(INT2FIX(m_sec(dat)),
			 m_sf_in_sec(dat)),
		   INT2FIX(m_of(dat)));
	return t;
    }
}
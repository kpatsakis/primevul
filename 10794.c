cmp_gen(VALUE self, VALUE other)
{
    get_d1(self);

    if (k_numeric_p(other))
	return INT2FIX(f_cmp(m_ajd(dat), other));
    else if (k_date_p(other))
	return INT2FIX(f_cmp(m_ajd(dat), f_ajd(other)));
    return rb_num_coerce_cmp(self, other, id_cmp);
}
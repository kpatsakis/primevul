m_zone(union DateData *x)
{
    if (simple_dat_p(x))
	return rb_usascii_str_new2("+00:00");
    return of2str(m_of(x));
}
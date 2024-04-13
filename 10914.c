m_sf(union DateData *x)
{
    if (simple_dat_p(x))
	return INT2FIX(0);
    else
	return x->c.sf;
}
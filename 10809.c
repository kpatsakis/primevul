m_virtual_sg(union DateData *x)
{
    if (simple_dat_p(x))
	return s_virtual_sg(x);
    else
	return c_virtual_sg(x);
}
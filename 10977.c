s_virtual_sg(union DateData *x)
{
    if (isinf(x->s.sg))
	return x->s.sg;
    if (f_zero_p(x->s.nth))
	return x->s.sg;
    else if (f_negative_p(x->s.nth))
	return positive_inf;
    return negative_inf;
}
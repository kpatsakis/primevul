lseg_dt(LSEG *l1, LSEG *l2)
{
	double		result,
				d;

	if (lseg_intersect_internal(l1, l2))
		return 0.0;

	d = dist_ps_internal(&l1->p[0], l2);
	result = d;
	d = dist_ps_internal(&l1->p[1], l2);
	result = Min(result, d);
	d = dist_ps_internal(&l2->p[0], l1);
	result = Min(result, d);
	d = dist_ps_internal(&l2->p[1], l1);
	result = Min(result, d);

	return result;
}

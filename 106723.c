lseg_perp(PG_FUNCTION_ARGS)
{
	LSEG	   *l1 = PG_GETARG_LSEG_P(0);
	LSEG	   *l2 = PG_GETARG_LSEG_P(1);
	double		m1,
				m2;

	m1 = point_sl(&(l1->p[0]), &(l1->p[1]));
	m2 = point_sl(&(l2->p[0]), &(l2->p[1]));

#ifdef GEODEBUG
	printf("lseg_perp- slopes are %g and %g\n", m1, m2);
#endif
	if (FPzero(m1))
		PG_RETURN_BOOL(FPeq(m2, DBL_MAX));
	else if (FPzero(m2))
		PG_RETURN_BOOL(FPeq(m1, DBL_MAX));

	PG_RETURN_BOOL(FPeq(m1 / m2, -1.0));
}

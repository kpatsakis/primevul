dist_cpoly(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(0);
	POLYGON    *poly = PG_GETARG_POLYGON_P(1);
	float8		result;
	float8		d;
	int			i;
	LSEG		seg;

	if (point_inside(&(circle->center), poly->npts, poly->p) != 0)
	{
#ifdef GEODEBUG
		printf("dist_cpoly- center inside of polygon\n");
#endif
		PG_RETURN_FLOAT8(0.0);
	}

	/* initialize distance with segment between first and last points */
	seg.p[0].x = poly->p[0].x;
	seg.p[0].y = poly->p[0].y;
	seg.p[1].x = poly->p[poly->npts - 1].x;
	seg.p[1].y = poly->p[poly->npts - 1].y;
	result = dist_ps_internal(&circle->center, &seg);
#ifdef GEODEBUG
	printf("dist_cpoly- segment 0/n distance is %f\n", result);
#endif

	/* check distances for other segments */
	for (i = 0; (i < poly->npts - 1); i++)
	{
		seg.p[0].x = poly->p[i].x;
		seg.p[0].y = poly->p[i].y;
		seg.p[1].x = poly->p[i + 1].x;
		seg.p[1].y = poly->p[i + 1].y;
		d = dist_ps_internal(&circle->center, &seg);
#ifdef GEODEBUG
		printf("dist_cpoly- segment %d distance is %f\n", (i + 1), d);
#endif
		if (d < result)
			result = d;
	}

	result -= circle->radius;
	if (result < 0)
		result = 0;

	PG_RETURN_FLOAT8(result);
}

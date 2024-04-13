poly_circle(PG_FUNCTION_ARGS)
{
	POLYGON    *poly = PG_GETARG_POLYGON_P(0);
	CIRCLE	   *circle;
	int			i;

	if (poly->npts < 2)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("cannot convert empty polygon to circle")));

	circle = (CIRCLE *) palloc(sizeof(CIRCLE));

	circle->center.x = 0;
	circle->center.y = 0;
	circle->radius = 0;

	for (i = 0; i < poly->npts; i++)
	{
		circle->center.x += poly->p[i].x;
		circle->center.y += poly->p[i].y;
	}
	circle->center.x /= poly->npts;
	circle->center.y /= poly->npts;

	for (i = 0; i < poly->npts; i++)
		circle->radius += point_dt(&poly->p[i], &circle->center);
	circle->radius /= poly->npts;

	if (FPzero(circle->radius))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("cannot convert empty polygon to circle")));

	PG_RETURN_CIRCLE_P(circle);
}

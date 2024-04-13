circle_poly(PG_FUNCTION_ARGS)
{
	int32		npts = PG_GETARG_INT32(0);
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(1);
	POLYGON    *poly;
	int			base_size,
				size;
	int			i;
	double		angle;
	double		anglestep;

	if (FPzero(circle->radius))
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			   errmsg("cannot convert circle with radius zero to polygon")));

	if (npts < 2)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("must request at least 2 points")));

	base_size = sizeof(poly->p[0]) * npts;
	size = offsetof(POLYGON, p[0]) +base_size;

	/* Check for integer overflow */
	if (base_size / npts != sizeof(poly->p[0]) || size <= base_size)
		ereport(ERROR,
				(errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
				 errmsg("too many points requested")));

	poly = (POLYGON *) palloc0(size);	/* zero any holes */
	SET_VARSIZE(poly, size);
	poly->npts = npts;

	anglestep = (2.0 * M_PI) / npts;

	for (i = 0; i < npts; i++)
	{
		angle = i * anglestep;
		poly->p[i].x = circle->center.x - (circle->radius * cos(angle));
		poly->p[i].y = circle->center.y + (circle->radius * sin(angle));
	}

	make_bound_box(poly);

	PG_RETURN_POLYGON_P(poly);
}

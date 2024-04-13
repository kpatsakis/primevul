path_area(PG_FUNCTION_ARGS)
{
	PATH	   *path = PG_GETARG_PATH_P(0);
	double		area = 0.0;
	int			i,
				j;

	if (!path->closed)
		PG_RETURN_NULL();

	for (i = 0; i < path->npts; i++)
	{
		j = (i + 1) % path->npts;
		area += path->p[i].x * path->p[j].y;
		area -= path->p[i].y * path->p[j].x;
	}

	area *= 0.5;
	PG_RETURN_FLOAT8(area < 0.0 ? -area : area);
}

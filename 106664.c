close_pl(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	LINE	   *line = PG_GETARG_LINE_P(1);
	Point	   *result;
	LINE	   *tmp;
	double		invm;

	result = (Point *) palloc(sizeof(Point));

	if (FPzero(line->B))		/* vertical? */
	{
		result->x = line->C;
		result->y = pt->y;
		PG_RETURN_POINT_P(result);
	}
	if (FPzero(line->A))		/* horizontal? */
	{
		result->x = pt->x;
		result->y = line->C;
		PG_RETURN_POINT_P(result);
	}
	/* drop a perpendicular and find the intersection point */

	/* invert and flip the sign on the slope to get a perpendicular */
	invm = line->B / line->A;
	tmp = line_construct_pm(pt, invm);
	result = line_interpt_internal(tmp, line);
	Assert(result != NULL);
	PG_RETURN_POINT_P(result);
}

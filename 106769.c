point_div(PG_FUNCTION_ARGS)
{
	Point	   *p1 = PG_GETARG_POINT_P(0);
	Point	   *p2 = PG_GETARG_POINT_P(1);
	Point	   *result;
	double		div;

	result = (Point *) palloc(sizeof(Point));

	div = (p2->x * p2->x) + (p2->y * p2->y);

	if (div == 0.0)
		ereport(ERROR,
				(errcode(ERRCODE_DIVISION_BY_ZERO),
				 errmsg("division by zero")));

	result->x = ((p1->x * p2->x) + (p1->y * p2->y)) / div;
	result->y = ((p2->x * p1->y) - (p2->y * p1->x)) / div;

	PG_RETURN_POINT_P(result);
}

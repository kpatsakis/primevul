point_mul(PG_FUNCTION_ARGS)
{
	Point	   *p1 = PG_GETARG_POINT_P(0);
	Point	   *p2 = PG_GETARG_POINT_P(1);
	Point	   *result;

	result = (Point *) palloc(sizeof(Point));

	result->x = (p1->x * p2->x) - (p1->y * p2->y);
	result->y = (p1->x * p2->y) + (p1->y * p2->x);

	PG_RETURN_POINT_P(result);
}

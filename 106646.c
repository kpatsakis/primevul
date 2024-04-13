circle_mul_pt(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(0);
	Point	   *point = PG_GETARG_POINT_P(1);
	CIRCLE	   *result;
	Point	   *p;

	result = circle_copy(circle);

	p = DatumGetPointP(DirectFunctionCall2(point_mul,
										   PointPGetDatum(&circle->center),
										   PointPGetDatum(point)));
	result->center.x = p->x;
	result->center.y = p->y;
	result->radius *= HYPOT(point->x, point->y);

	PG_RETURN_CIRCLE_P(result);
}

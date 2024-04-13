circle_sub_pt(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(0);
	Point	   *point = PG_GETARG_POINT_P(1);
	CIRCLE	   *result;

	result = circle_copy(circle);

	result->center.x -= point->x;
	result->center.y -= point->y;

	PG_RETURN_CIRCLE_P(result);
}

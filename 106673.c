dist_pc(PG_FUNCTION_ARGS)
{
	Point	   *point = PG_GETARG_POINT_P(0);
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(1);
	float8		result;

	result = point_dt(point, &circle->center) - circle->radius;
	if (result < 0)
		result = 0;
	PG_RETURN_FLOAT8(result);
}

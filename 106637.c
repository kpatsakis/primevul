circle_distance(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle1 = PG_GETARG_CIRCLE_P(0);
	CIRCLE	   *circle2 = PG_GETARG_CIRCLE_P(1);
	float8		result;

	result = point_dt(&circle1->center, &circle2->center)
		- (circle1->radius + circle2->radius);
	if (result < 0)
		result = 0;
	PG_RETURN_FLOAT8(result);
}

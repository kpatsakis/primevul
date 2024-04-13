circle_same(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle1 = PG_GETARG_CIRCLE_P(0);
	CIRCLE	   *circle2 = PG_GETARG_CIRCLE_P(1);

	PG_RETURN_BOOL(FPeq(circle1->radius, circle2->radius) &&
				   FPeq(circle1->center.x, circle2->center.x) &&
				   FPeq(circle1->center.y, circle2->center.y));
}

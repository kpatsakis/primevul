line_distance(PG_FUNCTION_ARGS)
{
	LINE	   *l1 = PG_GETARG_LINE_P(0);
	LINE	   *l2 = PG_GETARG_LINE_P(1);
	float8		result;
	Point	   *tmp;

	if (!DatumGetBool(DirectFunctionCall2(line_parallel,
										  LinePGetDatum(l1),
										  LinePGetDatum(l2))))
		PG_RETURN_FLOAT8(0.0);
	if (FPzero(l1->B))			/* vertical? */
		PG_RETURN_FLOAT8(fabs(l1->C - l2->C));
	tmp = point_construct(0.0, l1->C);
	result = dist_pl_internal(tmp, l2);
	PG_RETURN_FLOAT8(result);
}

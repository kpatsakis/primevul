line_intersect(PG_FUNCTION_ARGS)
{
	LINE	   *l1 = PG_GETARG_LINE_P(0);
	LINE	   *l2 = PG_GETARG_LINE_P(1);

	PG_RETURN_BOOL(!DatumGetBool(DirectFunctionCall2(line_parallel,
													 LinePGetDatum(l1),
													 LinePGetDatum(l2))));
}

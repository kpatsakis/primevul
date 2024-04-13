line_interpt(PG_FUNCTION_ARGS)
{
	LINE	   *l1 = PG_GETARG_LINE_P(0);
	LINE	   *l2 = PG_GETARG_LINE_P(1);
	Point	   *result;

	result = line_interpt_internal(l1, l2);

	if (result == NULL)
		PG_RETURN_NULL();
	PG_RETURN_POINT_P(result);
}

lseg_interpt(PG_FUNCTION_ARGS)
{
	LSEG	   *l1 = PG_GETARG_LSEG_P(0);
	LSEG	   *l2 = PG_GETARG_LSEG_P(1);
	Point	   *result;

	result = lseg_interpt_internal(l1, l2);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();

	PG_RETURN_POINT_P(result);
}

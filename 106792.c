poly_distance(PG_FUNCTION_ARGS)
{
#ifdef NOT_USED
	POLYGON    *polya = PG_GETARG_POLYGON_P(0);
	POLYGON    *polyb = PG_GETARG_POLYGON_P(1);
#endif

	ereport(ERROR,
			(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 errmsg("function \"poly_distance\" not implemented")));

	PG_RETURN_NULL();
}

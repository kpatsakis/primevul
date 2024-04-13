path_center(PG_FUNCTION_ARGS)
{
#ifdef NOT_USED
	PATH	   *path = PG_GETARG_PATH_P(0);
#endif

	ereport(ERROR,
			(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
			 errmsg("function \"path_center\" not implemented")));

	PG_RETURN_NULL();
}

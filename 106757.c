path_open(PG_FUNCTION_ARGS)
{
	PATH	   *path = PG_GETARG_PATH_P_COPY(0);

	path->closed = FALSE;

	PG_RETURN_PATH_P(path);
}

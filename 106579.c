ltxtq_in(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(queryin((char *) PG_GETARG_POINTER(0)));
}

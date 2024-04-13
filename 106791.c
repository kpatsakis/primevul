poly_contained(PG_FUNCTION_ARGS)
{
	Datum		polya = PG_GETARG_DATUM(0);
	Datum		polyb = PG_GETARG_DATUM(1);

	/* Just switch the arguments and pass it off to poly_contain */
	PG_RETURN_DATUM(DirectFunctionCall2(poly_contain, polyb, polya));
}

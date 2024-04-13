rboolop(PG_FUNCTION_ARGS)
{
	/* just reverse the operands */
	return DirectFunctionCall2(boolop,
							   PG_GETARG_DATUM(1),
							   PG_GETARG_DATUM(0));
}

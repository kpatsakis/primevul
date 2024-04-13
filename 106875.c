bitlength(PG_FUNCTION_ARGS)
{
	VarBit	   *arg = PG_GETARG_VARBIT_P(0);

	PG_RETURN_INT32(VARBITLEN(arg));
}

bitcat(PG_FUNCTION_ARGS)
{
	VarBit	   *arg1 = PG_GETARG_VARBIT_P(0);
	VarBit	   *arg2 = PG_GETARG_VARBIT_P(1);

	PG_RETURN_VARBIT_P(bit_catenate(arg1, arg2));
}

bitoverlay(PG_FUNCTION_ARGS)
{
	VarBit	   *t1 = PG_GETARG_VARBIT_P(0);
	VarBit	   *t2 = PG_GETARG_VARBIT_P(1);
	int			sp = PG_GETARG_INT32(2);		/* substring start position */
	int			sl = PG_GETARG_INT32(3);		/* substring length */

	PG_RETURN_VARBIT_P(bit_overlay(t1, t2, sp, sl));
}

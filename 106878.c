bitnot(PG_FUNCTION_ARGS)
{
	VarBit	   *arg = PG_GETARG_VARBIT_P(0);
	VarBit	   *result;
	bits8	   *p,
			   *r;
	bits8		mask;

	result = (VarBit *) palloc(VARSIZE(arg));
	SET_VARSIZE(result, VARSIZE(arg));
	VARBITLEN(result) = VARBITLEN(arg);

	p = VARBITS(arg);
	r = VARBITS(result);
	for (; p < VARBITEND(arg); p++)
		*r++ = ~*p;

	/* Pad the result */
	mask = BITMASK << VARBITPAD(result);
	if (mask)
	{
		r--;
		*r &= mask;
	}

	PG_RETURN_VARBIT_P(result);
}

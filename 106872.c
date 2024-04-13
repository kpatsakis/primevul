bitgetbit(PG_FUNCTION_ARGS)
{
	VarBit	   *arg1 = PG_GETARG_VARBIT_P(0);
	int32		n = PG_GETARG_INT32(1);
	int			bitlen;
	bits8	   *p;
	int			byteNo,
				bitNo;

	bitlen = VARBITLEN(arg1);
	if (n < 0 || n >= bitlen)
		ereport(ERROR,
				(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
				 errmsg("bit index %d out of valid range (0..%d)",
						n, bitlen - 1)));

	p = VARBITS(arg1);

	byteNo = n / BITS_PER_BYTE;
	bitNo = BITS_PER_BYTE - 1 - (n % BITS_PER_BYTE);

	if (p[byteNo] & (1 << bitNo))
		PG_RETURN_INT32(1);
	else
		PG_RETURN_INT32(0);
}

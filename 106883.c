bitsetbit(PG_FUNCTION_ARGS)
{
	VarBit	   *arg1 = PG_GETARG_VARBIT_P(0);
	int32		n = PG_GETARG_INT32(1);
	int32		newBit = PG_GETARG_INT32(2);
	VarBit	   *result;
	int			len,
				bitlen;
	bits8	   *r,
			   *p;
	int			byteNo,
				bitNo;

	bitlen = VARBITLEN(arg1);
	if (n < 0 || n >= bitlen)
		ereport(ERROR,
				(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
				 errmsg("bit index %d out of valid range (0..%d)",
						n, bitlen - 1)));

	/*
	 * sanity check!
	 */
	if (newBit != 0 && newBit != 1)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("new bit must be 0 or 1")));

	len = VARSIZE(arg1);
	result = (VarBit *) palloc(len);
	SET_VARSIZE(result, len);
	VARBITLEN(result) = bitlen;

	p = VARBITS(arg1);
	r = VARBITS(result);

	memcpy(r, p, VARBITBYTES(arg1));

	byteNo = n / BITS_PER_BYTE;
	bitNo = BITS_PER_BYTE - 1 - (n % BITS_PER_BYTE);

	/*
	 * Update the byte.
	 */
	if (newBit == 0)
		r[byteNo] &= (~(1 << bitNo));
	else
		r[byteNo] |= (1 << bitNo);

	PG_RETURN_VARBIT_P(result);
}

varbit(PG_FUNCTION_ARGS)
{
	VarBit	   *arg = PG_GETARG_VARBIT_P(0);
	int32		len = PG_GETARG_INT32(1);
	bool		isExplicit = PG_GETARG_BOOL(2);
	VarBit	   *result;
	int			rlen;
	int			ipad;
	bits8		mask;

	/* No work if typmod is invalid or supplied data matches it already */
	if (len <= 0 || len >= VARBITLEN(arg))
		PG_RETURN_VARBIT_P(arg);

	if (!isExplicit)
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_RIGHT_TRUNCATION),
				 errmsg("bit string too long for type bit varying(%d)",
						len)));

	rlen = VARBITTOTALLEN(len);
	result = (VarBit *) palloc(rlen);
	SET_VARSIZE(result, rlen);
	VARBITLEN(result) = len;

	memcpy(VARBITS(result), VARBITS(arg), VARBITBYTES(result));

	/* Make sure last byte is zero-padded if needed */
	ipad = VARBITPAD(result);
	if (ipad > 0)
	{
		mask = BITMASK << ipad;
		*(VARBITS(result) + VARBITBYTES(result) - 1) &= mask;
	}

	PG_RETURN_VARBIT_P(result);
}

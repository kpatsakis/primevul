bitshiftright(PG_FUNCTION_ARGS)
{
	VarBit	   *arg = PG_GETARG_VARBIT_P(0);
	int32		shft = PG_GETARG_INT32(1);
	VarBit	   *result;
	int			byte_shift,
				ishift,
				len;
	bits8	   *p,
			   *r;

	/* Negative shift is a shift to the left */
	if (shft < 0)
		PG_RETURN_DATUM(DirectFunctionCall2(bitshiftleft,
											VarBitPGetDatum(arg),
											Int32GetDatum(-shft)));

	result = (VarBit *) palloc(VARSIZE(arg));
	SET_VARSIZE(result, VARSIZE(arg));
	VARBITLEN(result) = VARBITLEN(arg);
	r = VARBITS(result);

	/* If we shifted all the bits out, return an all-zero string */
	if (shft >= VARBITLEN(arg))
	{
		MemSet(r, 0, VARBITBYTES(arg));
		PG_RETURN_VARBIT_P(result);
	}

	byte_shift = shft / BITS_PER_BYTE;
	ishift = shft % BITS_PER_BYTE;
	p = VARBITS(arg);

	/* Set the first part of the result to 0 */
	MemSet(r, 0, byte_shift);
	r += byte_shift;

	if (ishift == 0)
	{
		/* Special case: we can do a memcpy */
		len = VARBITBYTES(arg) - byte_shift;
		memcpy(r, p, len);
	}
	else
	{
		if (r < VARBITEND(result))
			*r = 0;				/* initialize first byte */
		for (; r < VARBITEND(result); p++)
		{
			*r |= *p >> ishift;
			if ((++r) < VARBITEND(result))
				*r = (*p << (BITS_PER_BYTE - ishift)) & BITMASK;
		}
	}

	PG_RETURN_VARBIT_P(result);
}

bit_out(PG_FUNCTION_ARGS)
{
#if 1
	/* same as varbit output */
	return varbit_out(fcinfo);
#else

	/*
	 * This is how one would print a hex string, in case someone wants to
	 * write a formatting function.
	 */
	VarBit	   *s = PG_GETARG_VARBIT_P(0);
	char	   *result,
			   *r;
	bits8	   *sp;
	int			i,
				len,
				bitlen;

	bitlen = VARBITLEN(s);
	len = (bitlen + 3) / 4;
	result = (char *) palloc(len + 2);
	sp = VARBITS(s);
	r = result;
	*r++ = 'X';
	/* we cheat by knowing that we store full bytes zero padded */
	for (i = 0; i < len; i += 2, sp++)
	{
		*r++ = HEXDIG((*sp) >> 4);
		*r++ = HEXDIG((*sp) & 0xF);
	}

	/*
	 * Go back one step if we printed a hex number that was not part of the
	 * bitstring anymore
	 */
	if (i > len)
		r--;
	*r = '\0';

	PG_RETURN_CSTRING(result);
#endif
}

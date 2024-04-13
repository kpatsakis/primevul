bit_overlay(VarBit *t1, VarBit *t2, int sp, int sl)
{
	VarBit	   *result;
	VarBit	   *s1;
	VarBit	   *s2;
	int			sp_pl_sl;

	/*
	 * Check for possible integer-overflow cases.  For negative sp, throw a
	 * "substring length" error because that's what should be expected
	 * according to the spec's definition of OVERLAY().
	 */
	if (sp <= 0)
		ereport(ERROR,
				(errcode(ERRCODE_SUBSTRING_ERROR),
				 errmsg("negative substring length not allowed")));
	sp_pl_sl = sp + sl;
	if (sp_pl_sl <= sl)
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("integer out of range")));

	s1 = bitsubstring(t1, 1, sp - 1, false);
	s2 = bitsubstring(t1, sp_pl_sl, -1, true);
	result = bit_catenate(s1, t2);
	result = bit_catenate(result, s2);

	return result;
}

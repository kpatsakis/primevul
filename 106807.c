single_encode(float8 x, char *str)
{
	int			ndig = DBL_DIG + extra_float_digits;

	if (ndig < 1)
		ndig = 1;

	sprintf(str, "%.*g", ndig, x);
	return TRUE;
}	/* single_encode() */

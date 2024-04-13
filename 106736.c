pair_encode(float8 x, float8 y, char *str)
{
	int			ndig = DBL_DIG + extra_float_digits;

	if (ndig < 1)
		ndig = 1;

	sprintf(str, "%.*g,%.*g", ndig, x, ndig, y);
	return TRUE;
}

single_decode(char *str, float8 *x, char **s)
{
	char	   *cp;

	if (!PointerIsValid(str))
		return FALSE;

	while (isspace((unsigned char) *str))
		str++;
	*x = strtod(str, &cp);
#ifdef GEODEBUG
	printf("single_decode- (%x) try decoding %s to %g\n", (cp - str), str, *x);
#endif
	if (cp <= str)
		return FALSE;
	while (isspace((unsigned char) *cp))
		cp++;

	if (s != NULL)
		*s = cp;

	return TRUE;
}	/* single_decode() */

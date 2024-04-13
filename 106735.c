pair_decode(char *str, float8 *x, float8 *y, char **s)
{
	int			has_delim;
	char	   *cp;

	if (!PointerIsValid(str))
		return FALSE;

	while (isspace((unsigned char) *str))
		str++;
	if ((has_delim = (*str == LDELIM)))
		str++;

	while (isspace((unsigned char) *str))
		str++;
	*x = strtod(str, &cp);
	if (cp <= str)
		return FALSE;
	while (isspace((unsigned char) *cp))
		cp++;
	if (*cp++ != DELIM)
		return FALSE;
	while (isspace((unsigned char) *cp))
		cp++;
	*y = strtod(cp, &str);
	if (str <= cp)
		return FALSE;
	while (isspace((unsigned char) *str))
		str++;
	if (has_delim)
	{
		if (*str != RDELIM)
			return FALSE;
		str++;
		while (isspace((unsigned char) *str))
			str++;
	}
	if (s != NULL)
		*s = str;

	return TRUE;
}

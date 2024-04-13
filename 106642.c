circle_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	CIRCLE	   *circle;
	char	   *s,
			   *cp;
	int			depth = 0;

	circle = (CIRCLE *) palloc(sizeof(CIRCLE));

	s = str;
	while (isspace((unsigned char) *s))
		s++;
	if ((*s == LDELIM_C) || (*s == LDELIM))
	{
		depth++;
		cp = (s + 1);
		while (isspace((unsigned char) *cp))
			cp++;
		if (*cp == LDELIM)
			s = cp;
	}

	if (!pair_decode(s, &circle->center.x, &circle->center.y, &s))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("invalid input syntax for type circle: \"%s\"", str)));

	if (*s == DELIM)
		s++;
	while (isspace((unsigned char) *s))
		s++;

	if ((!single_decode(s, &circle->radius, &s)) || (circle->radius < 0))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("invalid input syntax for type circle: \"%s\"", str)));

	while (depth > 0)
	{
		if ((*s == RDELIM)
			|| ((*s == RDELIM_C) && (depth == 1)))
		{
			depth--;
			s++;
			while (isspace((unsigned char) *s))
				s++;
		}
		else
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("invalid input syntax for type circle: \"%s\"", str)));
	}

	if (*s != '\0')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("invalid input syntax for type circle: \"%s\"", str)));

	PG_RETURN_CIRCLE_P(circle);
}

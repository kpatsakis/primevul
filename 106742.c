path_decode(int opentype, int npts, char *str, int *isopen, char **ss, Point *p)
{
	int			depth = 0;
	char	   *s,
			   *cp;
	int			i;

	s = str;
	while (isspace((unsigned char) *s))
		s++;
	if ((*isopen = (*s == LDELIM_EP)))
	{
		/* no open delimiter allowed? */
		if (!opentype)
			return FALSE;
		depth++;
		s++;
		while (isspace((unsigned char) *s))
			s++;

	}
	else if (*s == LDELIM)
	{
		cp = (s + 1);
		while (isspace((unsigned char) *cp))
			cp++;
		if (*cp == LDELIM)
		{
#ifdef NOT_USED
			/* nested delimiters with only one point? */
			if (npts <= 1)
				return FALSE;
#endif
			depth++;
			s = cp;
		}
		else if (strrchr(s, LDELIM) == s)
		{
			depth++;
			s = cp;
		}
	}

	for (i = 0; i < npts; i++)
	{
		if (!pair_decode(s, &(p->x), &(p->y), &s))
			return FALSE;

		if (*s == DELIM)
			s++;
		p++;
	}

	while (depth > 0)
	{
		if ((*s == RDELIM)
			|| ((*s == RDELIM_EP) && (*isopen) && (depth == 1)))
		{
			depth--;
			s++;
			while (isspace((unsigned char) *s))
				s++;
		}
		else
			return FALSE;
	}
	*ss = s;

	return TRUE;
}	/* path_decode() */

path_encode(enum path_delim path_delim, int npts, Point *pt)
{
	int			size = npts * (P_MAXLEN + 3) + 2;
	char	   *result;
	char	   *cp;
	int			i;

	/* Check for integer overflow */
	if ((size - 2) / npts != (P_MAXLEN + 3))
		ereport(ERROR,
				(errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
				 errmsg("too many points requested")));

	result = palloc(size);

	cp = result;
	switch (path_delim)
	{
		case PATH_CLOSED:
			*cp++ = LDELIM;
			break;
		case PATH_OPEN:
			*cp++ = LDELIM_EP;
			break;
		case PATH_NONE:
			break;
	}

	for (i = 0; i < npts; i++)
	{
		*cp++ = LDELIM;
		if (!pair_encode(pt->x, pt->y, cp))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("could not format \"path\" value")));

		cp += strlen(cp);
		*cp++ = RDELIM;
		*cp++ = DELIM;
		pt++;
	}
	cp--;
	switch (path_delim)
	{
		case PATH_CLOSED:
			*cp++ = RDELIM;
			break;
		case PATH_OPEN:
			*cp++ = RDELIM_EP;
			break;
		case PATH_NONE:
			break;
	}
	*cp = '\0';

	return result;
}	/* path_encode() */

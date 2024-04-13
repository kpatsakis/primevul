line_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	LINE	   *line;
	LSEG		lseg;
	int			isopen;
	char	   *s;

	line = (LINE *) palloc(sizeof(LINE));

	if (path_decode(TRUE, 2, str, &isopen, &s, &(lseg.p[0])) && *s == '\0')
	{
		if (FPeq(lseg.p[0].x, lseg.p[1].x) && FPeq(lseg.p[0].y, lseg.p[1].y))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					 errmsg("invalid line specification: must be two distinct points")));

		line_construct_pts(line, &lseg.p[0], &lseg.p[1]);
	}
	else if (line_decode(str, line))
	{
		if (FPzero(line->A) && FPzero(line->B))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					 errmsg("invalid line specification: A and B cannot both be zero")));
	}
	else
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type line: \"%s\"", str)));

	PG_RETURN_LINE_P(line);
}

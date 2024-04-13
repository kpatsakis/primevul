lseg_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	LSEG	   *lseg;
	int			isopen;
	char	   *s;

	lseg = (LSEG *) palloc(sizeof(LSEG));

	if ((!path_decode(TRUE, 2, str, &isopen, &s, &(lseg->p[0])))
		|| (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type lseg: \"%s\"", str)));

#ifdef NOT_USED
	lseg->m = point_sl(&lseg->p[0], &lseg->p[1]);
#endif

	PG_RETURN_LSEG_P(lseg);
}

circle_out(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(0);
	char	   *result;
	char	   *cp;

	result = palloc(2 * P_MAXLEN + 6);

	cp = result;
	*cp++ = LDELIM_C;
	*cp++ = LDELIM;
	if (!pair_encode(circle->center.x, circle->center.y, cp))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("could not format \"circle\" value")));

	cp += strlen(cp);
	*cp++ = RDELIM;
	*cp++ = DELIM;
	if (!single_encode(circle->radius, cp))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("could not format \"circle\" value")));

	cp += strlen(cp);
	*cp++ = RDELIM_C;
	*cp = '\0';

	PG_RETURN_CSTRING(result);
}

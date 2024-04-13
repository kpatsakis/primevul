dist_sl(PG_FUNCTION_ARGS)
{
	LSEG	   *lseg = PG_GETARG_LSEG_P(0);
	LINE	   *line = PG_GETARG_LINE_P(1);
	float8		result,
				d2;

	if (has_interpt_sl(lseg, line))
		result = 0.0;
	else
	{
		result = dist_pl_internal(&lseg->p[0], line);
		d2 = dist_pl_internal(&lseg->p[1], line);
		/* XXX shouldn't we take the min not max? */
		if (d2 > result)
			result = d2;
	}

	PG_RETURN_FLOAT8(result);
}

close_ls(PG_FUNCTION_ARGS)
{
	LINE	   *line = PG_GETARG_LINE_P(0);
	LSEG	   *lseg = PG_GETARG_LSEG_P(1);
	Point	   *result;
	float8		d1,
				d2;

	result = interpt_sl(lseg, line);
	if (result)
		PG_RETURN_POINT_P(result);

	d1 = dist_pl_internal(&lseg->p[0], line);
	d2 = dist_pl_internal(&lseg->p[1], line);
	if (d1 < d2)
		result = point_copy(&lseg->p[0]);
	else
		result = point_copy(&lseg->p[1]);

	PG_RETURN_POINT_P(result);
}

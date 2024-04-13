on_sl(PG_FUNCTION_ARGS)
{
	LSEG	   *lseg = PG_GETARG_LSEG_P(0);
	LINE	   *line = PG_GETARG_LINE_P(1);

	PG_RETURN_BOOL(DatumGetBool(DirectFunctionCall2(on_pl,
												 PointPGetDatum(&lseg->p[0]),
													LinePGetDatum(line))) &&
				   DatumGetBool(DirectFunctionCall2(on_pl,
												 PointPGetDatum(&lseg->p[1]),
													LinePGetDatum(line))));
}

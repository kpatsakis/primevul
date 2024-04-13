dist_sb(PG_FUNCTION_ARGS)
{
	LSEG	   *lseg = PG_GETARG_LSEG_P(0);
	BOX		   *box = PG_GETARG_BOX_P(1);
	Point	   *tmp;
	Datum		result;

	tmp = DatumGetPointP(DirectFunctionCall2(close_sb,
											 LsegPGetDatum(lseg),
											 BoxPGetDatum(box)));
	result = DirectFunctionCall2(dist_pb,
								 PointPGetDatum(tmp),
								 BoxPGetDatum(box));

	PG_RETURN_DATUM(result);
}

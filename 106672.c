dist_pb(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	BOX		   *box = PG_GETARG_BOX_P(1);
	float8		result;
	Point	   *near;

	near = DatumGetPointP(DirectFunctionCall2(close_pb,
											  PointPGetDatum(pt),
											  BoxPGetDatum(box)));
	result = point_dt(near, pt);

	PG_RETURN_FLOAT8(result);
}

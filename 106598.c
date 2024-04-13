box_div(PG_FUNCTION_ARGS)
{
	BOX		   *box = PG_GETARG_BOX_P(0);
	Point	   *p = PG_GETARG_POINT_P(1);
	BOX		   *result;
	Point	   *high,
			   *low;

	high = DatumGetPointP(DirectFunctionCall2(point_div,
											  PointPGetDatum(&box->high),
											  PointPGetDatum(p)));
	low = DatumGetPointP(DirectFunctionCall2(point_div,
											 PointPGetDatum(&box->low),
											 PointPGetDatum(p)));

	result = box_construct(high->x, low->x, high->y, low->y);

	PG_RETURN_BOX_P(result);
}

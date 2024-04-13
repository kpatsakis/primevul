close_pb(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	BOX		   *box = PG_GETARG_BOX_P(1);
	LSEG		lseg,
				seg;
	Point		point;
	double		dist,
				d;

	if (DatumGetBool(DirectFunctionCall2(on_pb,
										 PointPGetDatum(pt),
										 BoxPGetDatum(box))))
		PG_RETURN_POINT_P(pt);

	/* pairwise check lseg distances */
	point.x = box->low.x;
	point.y = box->high.y;
	statlseg_construct(&lseg, &box->low, &point);
	dist = dist_ps_internal(pt, &lseg);

	statlseg_construct(&seg, &box->high, &point);
	if ((d = dist_ps_internal(pt, &seg)) < dist)
	{
		dist = d;
		memcpy(&lseg, &seg, sizeof(lseg));
	}

	point.x = box->high.x;
	point.y = box->low.y;
	statlseg_construct(&seg, &box->low, &point);
	if ((d = dist_ps_internal(pt, &seg)) < dist)
	{
		dist = d;
		memcpy(&lseg, &seg, sizeof(lseg));
	}

	statlseg_construct(&seg, &box->high, &point);
	if ((d = dist_ps_internal(pt, &seg)) < dist)
	{
		dist = d;
		memcpy(&lseg, &seg, sizeof(lseg));
	}

	PG_RETURN_DATUM(DirectFunctionCall2(close_ps,
										PointPGetDatum(pt),
										LsegPGetDatum(&lseg)));
}

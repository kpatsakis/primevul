inter_sb(PG_FUNCTION_ARGS)
{
	LSEG	   *lseg = PG_GETARG_LSEG_P(0);
	BOX		   *box = PG_GETARG_BOX_P(1);
	BOX			lbox;
	LSEG		bseg;
	Point		point;

	lbox.low.x = Min(lseg->p[0].x, lseg->p[1].x);
	lbox.low.y = Min(lseg->p[0].y, lseg->p[1].y);
	lbox.high.x = Max(lseg->p[0].x, lseg->p[1].x);
	lbox.high.y = Max(lseg->p[0].y, lseg->p[1].y);

	/* nothing close to overlap? then not going to intersect */
	if (!box_ov(&lbox, box))
		PG_RETURN_BOOL(false);

	/* an endpoint of segment is inside box? then clearly intersects */
	if (DatumGetBool(DirectFunctionCall2(on_pb,
										 PointPGetDatum(&lseg->p[0]),
										 BoxPGetDatum(box))) ||
		DatumGetBool(DirectFunctionCall2(on_pb,
										 PointPGetDatum(&lseg->p[1]),
										 BoxPGetDatum(box))))
		PG_RETURN_BOOL(true);

	/* pairwise check lseg intersections */
	point.x = box->low.x;
	point.y = box->high.y;
	statlseg_construct(&bseg, &box->low, &point);
	if (lseg_intersect_internal(&bseg, lseg))
		PG_RETURN_BOOL(true);

	statlseg_construct(&bseg, &box->high, &point);
	if (lseg_intersect_internal(&bseg, lseg))
		PG_RETURN_BOOL(true);

	point.x = box->high.x;
	point.y = box->low.y;
	statlseg_construct(&bseg, &box->low, &point);
	if (lseg_intersect_internal(&bseg, lseg))
		PG_RETURN_BOOL(true);

	statlseg_construct(&bseg, &box->high, &point);
	if (lseg_intersect_internal(&bseg, lseg))
		PG_RETURN_BOOL(true);

	/* if we dropped through, no two segs intersected */
	PG_RETURN_BOOL(false);
}

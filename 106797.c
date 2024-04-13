poly_overlap(PG_FUNCTION_ARGS)
{
	POLYGON    *polya = PG_GETARG_POLYGON_P(0);
	POLYGON    *polyb = PG_GETARG_POLYGON_P(1);
	bool		result;

	/* Quick check by bounding box */
	result = (polya->npts > 0 && polyb->npts > 0 &&
			  box_ov(&polya->boundbox, &polyb->boundbox)) ? true : false;

	/*
	 * Brute-force algorithm - try to find intersected edges, if so then
	 * polygons are overlapped else check is one polygon inside other or not
	 * by testing single point of them.
	 */
	if (result)
	{
		int			ia,
					ib;
		LSEG		sa,
					sb;

		/* Init first of polya's edge with last point */
		sa.p[0] = polya->p[polya->npts - 1];
		result = false;

		for (ia = 0; ia < polya->npts && result == false; ia++)
		{
			/* Second point of polya's edge is a current one */
			sa.p[1] = polya->p[ia];

			/* Init first of polyb's edge with last point */
			sb.p[0] = polyb->p[polyb->npts - 1];

			for (ib = 0; ib < polyb->npts && result == false; ib++)
			{
				sb.p[1] = polyb->p[ib];
				result = lseg_intersect_internal(&sa, &sb);
				sb.p[0] = sb.p[1];
			}

			/*
			 * move current endpoint to the first point of next edge
			 */
			sa.p[0] = sa.p[1];
		}

		if (result == false)
		{
			result = (point_inside(polya->p, polyb->npts, polyb->p)
					  ||
					  point_inside(polyb->p, polya->npts, polya->p));
		}
	}

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(polya, 0);
	PG_FREE_IF_COPY(polyb, 1);

	PG_RETURN_BOOL(result);
}

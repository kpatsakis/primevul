dist_ps_internal(Point *pt, LSEG *lseg)
{
	double		m;				/* slope of perp. */
	LINE	   *ln;
	double		result,
				tmpdist;
	Point	   *ip;

	/*
	 * Construct a line perpendicular to the input segment and through the
	 * input point
	 */
	if (lseg->p[1].x == lseg->p[0].x)
		m = 0;
	else if (lseg->p[1].y == lseg->p[0].y)
		m = (double) DBL_MAX;	/* slope is infinite */
	else
		m = (lseg->p[0].x - lseg->p[1].x) / (lseg->p[1].y - lseg->p[0].y);
	ln = line_construct_pm(pt, m);

#ifdef GEODEBUG
	printf("dist_ps- line is A=%g B=%g C=%g from (point) slope (%f,%f) %g\n",
		   ln->A, ln->B, ln->C, pt->x, pt->y, m);
#endif

	/*
	 * Calculate distance to the line segment or to the nearest endpoint of
	 * the segment.
	 */

	/* intersection is on the line segment? */
	if ((ip = interpt_sl(lseg, ln)) != NULL)
	{
		/* yes, so use distance to the intersection point */
		result = point_dt(pt, ip);
#ifdef GEODEBUG
		printf("dist_ps- distance is %f to intersection point is (%f,%f)\n",
			   result, ip->x, ip->y);
#endif
	}
	else
	{
		/* no, so use distance to the nearer endpoint */
		result = point_dt(pt, &lseg->p[0]);
		tmpdist = point_dt(pt, &lseg->p[1]);
		if (tmpdist < result)
			result = tmpdist;
	}

	return result;
}

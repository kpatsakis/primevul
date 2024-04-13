lseg_inside_poly(Point *a, Point *b, POLYGON *poly, int start)
{
	LSEG		s,
				t;
	int			i;
	bool		res = true,
				intersection = false;

	t.p[0] = *a;
	t.p[1] = *b;
	s.p[0] = poly->p[(start == 0) ? (poly->npts - 1) : (start - 1)];

	for (i = start; i < poly->npts && res; i++)
	{
		Point	   *interpt;

		s.p[1] = poly->p[i];

		if (on_ps_internal(t.p, &s))
		{
			if (on_ps_internal(t.p + 1, &s))
				return true;	/* t is contained by s */

			/* Y-cross */
			res = touched_lseg_inside_poly(t.p, t.p + 1, &s, poly, i + 1);
		}
		else if (on_ps_internal(t.p + 1, &s))
		{
			/* Y-cross */
			res = touched_lseg_inside_poly(t.p + 1, t.p, &s, poly, i + 1);
		}
		else if ((interpt = lseg_interpt_internal(&t, &s)) != NULL)
		{
			/*
			 * segments are X-crossing, go to check each subsegment
			 */

			intersection = true;
			res = lseg_inside_poly(t.p, interpt, poly, i + 1);
			if (res)
				res = lseg_inside_poly(t.p + 1, interpt, poly, i + 1);
			pfree(interpt);
		}

		s.p[0] = s.p[1];
	}

	if (res && !intersection)
	{
		Point		p;

		/*
		 * if X-intersection wasn't found  then check central point of tested
		 * segment. In opposite case we already check all subsegments
		 */
		p.x = (t.p[0].x + t.p[1].x) / 2.0;
		p.y = (t.p[0].y + t.p[1].y) / 2.0;

		res = point_inside(&p, poly->npts, poly->p);
	}

	return res;
}

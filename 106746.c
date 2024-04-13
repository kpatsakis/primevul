path_inter(PG_FUNCTION_ARGS)
{
	PATH	   *p1 = PG_GETARG_PATH_P(0);
	PATH	   *p2 = PG_GETARG_PATH_P(1);
	BOX			b1,
				b2;
	int			i,
				j;
	LSEG		seg1,
				seg2;

	if (p1->npts <= 0 || p2->npts <= 0)
		PG_RETURN_BOOL(false);

	b1.high.x = b1.low.x = p1->p[0].x;
	b1.high.y = b1.low.y = p1->p[0].y;
	for (i = 1; i < p1->npts; i++)
	{
		b1.high.x = Max(p1->p[i].x, b1.high.x);
		b1.high.y = Max(p1->p[i].y, b1.high.y);
		b1.low.x = Min(p1->p[i].x, b1.low.x);
		b1.low.y = Min(p1->p[i].y, b1.low.y);
	}
	b2.high.x = b2.low.x = p2->p[0].x;
	b2.high.y = b2.low.y = p2->p[0].y;
	for (i = 1; i < p2->npts; i++)
	{
		b2.high.x = Max(p2->p[i].x, b2.high.x);
		b2.high.y = Max(p2->p[i].y, b2.high.y);
		b2.low.x = Min(p2->p[i].x, b2.low.x);
		b2.low.y = Min(p2->p[i].y, b2.low.y);
	}
	if (!box_ov(&b1, &b2))
		PG_RETURN_BOOL(false);

	/* pairwise check lseg intersections */
	for (i = 0; i < p1->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!p1->closed)
				continue;
			iprev = p1->npts - 1;		/* include the closure segment */
		}

		for (j = 0; j < p2->npts; j++)
		{
			int			jprev;

			if (j > 0)
				jprev = j - 1;
			else
			{
				if (!p2->closed)
					continue;
				jprev = p2->npts - 1;	/* include the closure segment */
			}

			statlseg_construct(&seg1, &p1->p[iprev], &p1->p[i]);
			statlseg_construct(&seg2, &p2->p[jprev], &p2->p[j]);
			if (lseg_intersect_internal(&seg1, &seg2))
				PG_RETURN_BOOL(true);
		}
	}

	/* if we dropped through, no two segs intersected */
	PG_RETURN_BOOL(false);
}

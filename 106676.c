dist_ppath(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	PATH	   *path = PG_GETARG_PATH_P(1);
	float8		result = 0.0;	/* keep compiler quiet */
	bool		have_min = false;
	float8		tmp;
	int			i;
	LSEG		lseg;

	switch (path->npts)
	{
		case 0:
			/* no points in path? then result is undefined... */
			PG_RETURN_NULL();
		case 1:
			/* one point in path? then get distance between two points... */
			result = point_dt(pt, &path->p[0]);
			break;
		default:
			/* make sure the path makes sense... */
			Assert(path->npts > 1);

			/*
			 * the distance from a point to a path is the smallest distance
			 * from the point to any of its constituent segments.
			 */
			for (i = 0; i < path->npts; i++)
			{
				int			iprev;

				if (i > 0)
					iprev = i - 1;
				else
				{
					if (!path->closed)
						continue;
					iprev = path->npts - 1;		/* include the closure segment */
				}

				statlseg_construct(&lseg, &path->p[iprev], &path->p[i]);
				tmp = dist_ps_internal(pt, &lseg);
				if (!have_min || tmp < result)
				{
					result = tmp;
					have_min = true;
				}
			}
			break;
	}
	PG_RETURN_FLOAT8(result);
}

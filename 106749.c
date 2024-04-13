path_length(PG_FUNCTION_ARGS)
{
	PATH	   *path = PG_GETARG_PATH_P(0);
	float8		result = 0.0;
	int			i;

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

		result += point_dt(&path->p[iprev], &path->p[i]);
	}

	PG_RETURN_FLOAT8(result);
}

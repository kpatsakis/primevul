path_sub_pt(PG_FUNCTION_ARGS)
{
	PATH	   *path = PG_GETARG_PATH_P_COPY(0);
	Point	   *point = PG_GETARG_POINT_P(1);
	int			i;

	for (i = 0; i < path->npts; i++)
	{
		path->p[i].x -= point->x;
		path->p[i].y -= point->y;
	}

	PG_RETURN_PATH_P(path);
}

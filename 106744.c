path_div_pt(PG_FUNCTION_ARGS)
{
	PATH	   *path = PG_GETARG_PATH_P_COPY(0);
	Point	   *point = PG_GETARG_POINT_P(1);
	Point	   *p;
	int			i;

	for (i = 0; i < path->npts; i++)
	{
		p = DatumGetPointP(DirectFunctionCall2(point_div,
											   PointPGetDatum(&path->p[i]),
											   PointPGetDatum(point)));
		path->p[i].x = p->x;
		path->p[i].y = p->y;
	}

	PG_RETURN_PATH_P(path);
}

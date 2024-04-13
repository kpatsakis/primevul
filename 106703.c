lseg_center(PG_FUNCTION_ARGS)
{
	LSEG	   *lseg = PG_GETARG_LSEG_P(0);
	Point	   *result;

	result = (Point *) palloc(sizeof(Point));

	result->x = (lseg->p[0].x + lseg->p[1].x) / 2.0;
	result->y = (lseg->p[0].y + lseg->p[1].y) / 2.0;

	PG_RETURN_POINT_P(result);
}

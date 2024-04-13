point_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Point	   *point;
	double		x,
				y;
	char	   *s;

	if (!pair_decode(str, &x, &y, &s) || (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type point: \"%s\"", str)));

	point = (Point *) palloc(sizeof(Point));

	point->x = x;
	point->y = y;

	PG_RETURN_POINT_P(point);
}

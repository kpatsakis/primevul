point_construct(double x, double y)
{
	Point	   *result = (Point *) palloc(sizeof(Point));

	result->x = x;
	result->y = y;
	return result;
}

point_copy(Point *pt)
{
	Point	   *result;

	if (!PointerIsValid(pt))
		return NULL;

	result = (Point *) palloc(sizeof(Point));

	result->x = pt->x;
	result->y = pt->y;
	return result;
}

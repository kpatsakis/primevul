box_construct(double x1, double x2, double y1, double y2)
{
	BOX		   *result = (BOX *) palloc(sizeof(BOX));

	return box_fill(result, x1, x2, y1, y2);
}

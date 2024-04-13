box_fill(BOX *result, double x1, double x2, double y1, double y2)
{
	if (x1 > x2)
	{
		result->high.x = x1;
		result->low.x = x2;
	}
	else
	{
		result->high.x = x2;
		result->low.x = x1;
	}
	if (y1 > y2)
	{
		result->high.y = y1;
		result->low.y = y2;
	}
	else
	{
		result->high.y = y2;
		result->low.y = y1;
	}

	return result;
}

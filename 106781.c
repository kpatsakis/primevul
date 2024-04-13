point_sl(Point *pt1, Point *pt2)
{
	return (FPeq(pt1->x, pt2->x)
			? (double) DBL_MAX
			: (pt1->y - pt2->y) / (pt1->x - pt2->x));
}

point_dt(Point *pt1, Point *pt2)
{
#ifdef GEODEBUG
	printf("point_dt- segment (%f,%f),(%f,%f) length is %f\n",
	pt1->x, pt1->y, pt2->x, pt2->y, HYPOT(pt1->x - pt2->x, pt1->y - pt2->y));
#endif
	return HYPOT(pt1->x - pt2->x, pt1->y - pt2->y);
}

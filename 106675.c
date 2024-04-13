dist_pl_internal(Point *pt, LINE *line)
{
	return fabs((line->A * pt->x + line->B * pt->y + line->C) /
				HYPOT(line->A, line->B));
}

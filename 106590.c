box_cn(Point *center, BOX *box)
{
	center->x = (box->high.x + box->low.x) / 2.0;
	center->y = (box->high.y + box->low.y) / 2.0;
}

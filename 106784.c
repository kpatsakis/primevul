points_box(PG_FUNCTION_ARGS)
{
	Point	   *p1 = PG_GETARG_POINT_P(0);
	Point	   *p2 = PG_GETARG_POINT_P(1);

	PG_RETURN_BOX_P(box_construct(p1->x, p2->x, p1->y, p2->y));
}

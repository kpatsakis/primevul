box_sub(PG_FUNCTION_ARGS)
{
	BOX		   *box = PG_GETARG_BOX_P(0);
	Point	   *p = PG_GETARG_POINT_P(1);

	PG_RETURN_BOX_P(box_construct((box->high.x - p->x),
								  (box->low.x - p->x),
								  (box->high.y - p->y),
								  (box->low.y - p->y)));
}

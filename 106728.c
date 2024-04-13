on_pb(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	BOX		   *box = PG_GETARG_BOX_P(1);

	PG_RETURN_BOOL(pt->x <= box->high.x && pt->x >= box->low.x &&
				   pt->y <= box->high.y && pt->y >= box->low.y);
}

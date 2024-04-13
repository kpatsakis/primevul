box_contained(PG_FUNCTION_ARGS)
{
	BOX		   *box1 = PG_GETARG_BOX_P(0);
	BOX		   *box2 = PG_GETARG_BOX_P(1);

	PG_RETURN_BOOL(FPle(box1->high.x, box2->high.x) &&
				   FPge(box1->low.x, box2->low.x) &&
				   FPle(box1->high.y, box2->high.y) &&
				   FPge(box1->low.y, box2->low.y));
}

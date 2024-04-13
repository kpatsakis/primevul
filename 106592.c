box_contain(PG_FUNCTION_ARGS)
{
	BOX		   *box1 = PG_GETARG_BOX_P(0);
	BOX		   *box2 = PG_GETARG_BOX_P(1);

	PG_RETURN_BOOL(FPge(box1->high.x, box2->high.x) &&
				   FPle(box1->low.x, box2->low.x) &&
				   FPge(box1->high.y, box2->high.y) &&
				   FPle(box1->low.y, box2->low.y));
}

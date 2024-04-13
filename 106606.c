box_intersect(PG_FUNCTION_ARGS)
{
	BOX		   *box1 = PG_GETARG_BOX_P(0);
	BOX		   *box2 = PG_GETARG_BOX_P(1);
	BOX		   *result;

	if (!box_ov(box1, box2))
		PG_RETURN_NULL();

	result = (BOX *) palloc(sizeof(BOX));

	result->high.x = Min(box1->high.x, box2->high.x);
	result->low.x = Max(box1->low.x, box2->low.x);
	result->high.y = Min(box1->high.y, box2->high.y);
	result->low.y = Max(box1->low.y, box2->low.y);

	PG_RETURN_BOX_P(result);
}

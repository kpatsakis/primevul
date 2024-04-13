circle_box(PG_FUNCTION_ARGS)
{
	CIRCLE	   *circle = PG_GETARG_CIRCLE_P(0);
	BOX		   *box;
	double		delta;

	box = (BOX *) palloc(sizeof(BOX));

	delta = circle->radius / sqrt(2.0);

	box->high.x = circle->center.x + delta;
	box->low.x = circle->center.x - delta;
	box->high.y = circle->center.y + delta;
	box->low.y = circle->center.y - delta;

	PG_RETURN_BOX_P(box);
}

poly_box(PG_FUNCTION_ARGS)
{
	POLYGON    *poly = PG_GETARG_POLYGON_P(0);
	BOX		   *box;

	if (poly->npts < 1)
		PG_RETURN_NULL();

	box = box_copy(&poly->boundbox);

	PG_RETURN_BOX_P(box);
}

on_pl(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	LINE	   *line = PG_GETARG_LINE_P(1);

	PG_RETURN_BOOL(FPzero(line->A * pt->x + line->B * pt->y + line->C));
}

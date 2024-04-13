line_perp(PG_FUNCTION_ARGS)
{
	LINE	   *l1 = PG_GETARG_LINE_P(0);
	LINE	   *l2 = PG_GETARG_LINE_P(1);

	if (FPzero(l1->A))
		PG_RETURN_BOOL(FPzero(l2->B));
	else if (FPzero(l1->B))
		PG_RETURN_BOOL(FPzero(l2->A));

	PG_RETURN_BOOL(FPeq(((l1->A * l2->B) / (l1->B * l2->A)), -1.0));
}

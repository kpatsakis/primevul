line_parallel(PG_FUNCTION_ARGS)
{
	LINE	   *l1 = PG_GETARG_LINE_P(0);
	LINE	   *l2 = PG_GETARG_LINE_P(1);

	if (FPzero(l1->B))
		PG_RETURN_BOOL(FPzero(l2->B));

	PG_RETURN_BOOL(FPeq(l2->A, l1->A * (l2->B / l1->B)));
}

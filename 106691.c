line_eq(PG_FUNCTION_ARGS)
{
	LINE	   *l1 = PG_GETARG_LINE_P(0);
	LINE	   *l2 = PG_GETARG_LINE_P(1);
	double		k;

	if (!FPzero(l2->A))
		k = l1->A / l2->A;
	else if (!FPzero(l2->B))
		k = l1->B / l2->B;
	else if (!FPzero(l2->C))
		k = l1->C / l2->C;
	else
		k = 1.0;

	PG_RETURN_BOOL(FPeq(l1->A, k * l2->A) &&
				   FPeq(l1->B, k * l2->B) &&
				   FPeq(l1->C, k * l2->C));
}

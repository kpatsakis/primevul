lseg_eq(PG_FUNCTION_ARGS)
{
	LSEG	   *l1 = PG_GETARG_LSEG_P(0);
	LSEG	   *l2 = PG_GETARG_LSEG_P(1);

	PG_RETURN_BOOL(FPeq(l1->p[0].x, l2->p[0].x) &&
				   FPeq(l1->p[0].y, l2->p[0].y) &&
				   FPeq(l1->p[1].x, l2->p[1].x) &&
				   FPeq(l1->p[1].y, l2->p[1].y));
}

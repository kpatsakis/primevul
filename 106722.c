lseg_parallel(PG_FUNCTION_ARGS)
{
	LSEG	   *l1 = PG_GETARG_LSEG_P(0);
	LSEG	   *l2 = PG_GETARG_LSEG_P(1);

#ifdef NOT_USED
	PG_RETURN_BOOL(FPeq(l1->m, l2->m));
#endif
	PG_RETURN_BOOL(FPeq(point_sl(&l1->p[0], &l1->p[1]),
						point_sl(&l2->p[0], &l2->p[1])));
}

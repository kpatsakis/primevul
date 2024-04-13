on_ps_internal(Point *pt, LSEG *lseg)
{
	return FPeq(point_dt(pt, &lseg->p[0]) + point_dt(pt, &lseg->p[1]),
				point_dt(&lseg->p[0], &lseg->p[1]));
}

interpt_sl(LSEG *lseg, LINE *line)
{
	LINE		tmp;
	Point	   *p;

	line_construct_pts(&tmp, &lseg->p[0], &lseg->p[1]);
	p = line_interpt_internal(&tmp, line);
#ifdef GEODEBUG
	printf("interpt_sl- segment is (%.*g %.*g) (%.*g %.*g)\n",
		   DBL_DIG, lseg->p[0].x, DBL_DIG, lseg->p[0].y, DBL_DIG, lseg->p[1].x, DBL_DIG, lseg->p[1].y);
	printf("interpt_sl- segment becomes line A=%.*g B=%.*g C=%.*g\n",
		   DBL_DIG, tmp.A, DBL_DIG, tmp.B, DBL_DIG, tmp.C);
#endif
	if (PointerIsValid(p))
	{
#ifdef GEODEBUG
		printf("interpt_sl- intersection point is (%.*g %.*g)\n", DBL_DIG, p->x, DBL_DIG, p->y);
#endif
		if (on_ps_internal(p, lseg))
		{
#ifdef GEODEBUG
			printf("interpt_sl- intersection point is on segment\n");
#endif
		}
		else
			p = NULL;
	}

	return p;
}

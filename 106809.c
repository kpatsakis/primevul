touched_lseg_inside_poly(Point *a, Point *b, LSEG *s, POLYGON *poly, int start)
{
	/* point a is on s, b is not */
	LSEG		t;

	t.p[0] = *a;
	t.p[1] = *b;

#define POINTEQ(pt1, pt2)	(FPeq((pt1)->x, (pt2)->x) && FPeq((pt1)->y, (pt2)->y))
	if (POINTEQ(a, s->p))
	{
		if (on_ps_internal(s->p + 1, &t))
			return lseg_inside_poly(b, s->p + 1, poly, start);
	}
	else if (POINTEQ(a, s->p + 1))
	{
		if (on_ps_internal(s->p, &t))
			return lseg_inside_poly(b, s->p, poly, start);
	}
	else if (on_ps_internal(s->p, &t))
	{
		return lseg_inside_poly(b, s->p, poly, start);
	}
	else if (on_ps_internal(s->p + 1, &t))
	{
		return lseg_inside_poly(b, s->p + 1, poly, start);
	}

	return true;				/* may be not true, but that will check later */
}

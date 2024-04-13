lseg_interpt_internal(LSEG *l1, LSEG *l2)
{
	Point	   *result;
	LINE		tmp1,
				tmp2;

	/*
	 * Find the intersection of the appropriate lines, if any.
	 */
	line_construct_pts(&tmp1, &l1->p[0], &l1->p[1]);
	line_construct_pts(&tmp2, &l2->p[0], &l2->p[1]);
	result = line_interpt_internal(&tmp1, &tmp2);
	if (!PointerIsValid(result))
		return NULL;

	/*
	 * If the line intersection point isn't within l1 (or equivalently l2),
	 * there is no valid segment intersection point at all.
	 */
	if (!on_ps_internal(result, l1) ||
		!on_ps_internal(result, l2))
	{
		pfree(result);
		return NULL;
	}

	/*
	 * If there is an intersection, then check explicitly for matching
	 * endpoints since there may be rounding effects with annoying lsb
	 * residue. - tgl 1997-07-09
	 */
	if ((FPeq(l1->p[0].x, l2->p[0].x) && FPeq(l1->p[0].y, l2->p[0].y)) ||
		(FPeq(l1->p[0].x, l2->p[1].x) && FPeq(l1->p[0].y, l2->p[1].y)))
	{
		result->x = l1->p[0].x;
		result->y = l1->p[0].y;
	}
	else if ((FPeq(l1->p[1].x, l2->p[0].x) && FPeq(l1->p[1].y, l2->p[0].y)) ||
			 (FPeq(l1->p[1].x, l2->p[1].x) && FPeq(l1->p[1].y, l2->p[1].y)))
	{
		result->x = l1->p[1].x;
		result->y = l1->p[1].y;
	}

	return result;
}
